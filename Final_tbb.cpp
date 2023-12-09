using namespace std; 
using namespace tbb;
#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <tbb/tbb.h>


class Max {
public:
 float *my_a;
 float my_max;
 int my_index;

 void operator() (const blocked_range<size_t> &r) {
 float *a = my_a;
 float max = my_max;
 int index = 0; 
 // Get the maximum for a portion of the vector:
 for (size_t i = r.begin(); i!= r.end(); i++) 
    { 
        if (a[i] >= max) 
            {
                max = a[i];
                index = i;

            } 
    }   
 my_max = max;
 my_index = index;
 }

 Max (Max &x, split): my_a (x.my_a), my_max(x.my_a[0]), my_index(0) {}
 void join (const Max &y) { if (y.my_max > my_max) my_max = y.my_max; my_index = y.my_index;} //get max of two subresults
 Max (float *a): my_a(a), my_max(my_a[0]), my_index(0) {} // my_max = my_a[0]
 Max () {}
};

class Min {
public:
 float *my_a;
 float my_min;
 int my_index;

 void operator() (const blocked_range<size_t> &r) {
 float *a = my_a;
 float min = my_min;
 int index = 0;
 // Get the minimum for a portion of the vector:
 for (size_t i = r.begin(); i!= r.end(); i++) 
    { 
        if (a[i] <= min) 
            {   
                min = a[i];
                index = i;
                
            }
    }
 my_min = min;
 my_index = index;
 }

 Min (Min &x, split): my_a (x.my_a), my_min(x.my_a[0]), my_index(0) {}
 void join (const Min &y) { if (y.my_min < my_min) my_min = y.my_min; my_index = y.my_index; } //get max of two subresults
 Min (float *a): my_a(a), my_min(my_a[0]), my_index(0) {} // my_max = my_a[0]
 Min () {}
};

float getmax_tbb (float *a, size_t L)
{ 
    Max max(a);
    tbb::parallel_reduce (blocked_range<size_t> (0,L),max);
    return max.my_index;
}

float getmin_tbb (float *a, size_t L)
{ 
    Min min(a);
    tbb::parallel_reduce (blocked_range<size_t> (0,L),min);
    return min.my_index;
}



typedef struct 
{
    float *timestamp;
    int *PID;
    float *Data;
    int Data_Length;
} DATA1;


typedef struct 
{
    float timestamp;
    int PID;
    float Data;
}   tmp_data;



int main() {

    FILE *file_o1;
    FILE *file_o2;
    char *out_file1 = "engine_speed_histogram.bof";
    char *out_file2 = "vehicle_speed_histogram.bof";

    int n = 305000;
    
    DATA1 Engine_Speed;
    int histogram_engine_speed_binsize = 500;
    int histogram_engine_speed[8000/histogram_engine_speed_binsize]; 
    for(int i = 0; i <8000/histogram_engine_speed_binsize; i++)
    {
        histogram_engine_speed[i] = 0;
    }

    DATA1 Vehicle_Speed;
    int histogram_vehicle_speed_binsize = 10;
    int histogram_vehicle_speed[160/histogram_vehicle_speed_binsize]; 
    for(int i = 0; i <160/histogram_vehicle_speed_binsize; i++)
    {
        histogram_vehicle_speed[i] = 0;
    }


    DATA1 ECT;
    DATA1 Fuel_Percent;
    DATA1 Distance_Since_Clear;

    struct timeval start, end;
    long t_us;
    
    //we need to look through the text document and count how many of each DTC identifier there is 

    tmp_data TEMP_DATA_Input;
  

    const char* filename = "data.txt";  // Change this to your file name ....

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    
    int count = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int count5 = 0;
    Engine_Speed.Data_Length = 0;
    Vehicle_Speed.Data_Length = 0;
    ECT.Data_Length = 0;
    Fuel_Percent.Data_Length = 0;
    Distance_Since_Clear.Data_Length = 0;




    while (file >> TEMP_DATA_Input.timestamp >> TEMP_DATA_Input.PID >> TEMP_DATA_Input.Data) {
        // Assuming your struct has a public array and you want to store each entry
    
        if (TEMP_DATA_Input.PID == 12)
        {
        Engine_Speed.Data_Length ++;
        }

        else if (TEMP_DATA_Input.PID == 13)
        {
        Vehicle_Speed.Data_Length ++;
        }
        
        else if (TEMP_DATA_Input.PID == 5)
        {
        ECT.Data_Length ++;
        }
        
        else if (TEMP_DATA_Input.PID == 47)
        {
        Fuel_Percent.Data_Length ++;
        }

        else if (TEMP_DATA_Input.PID == 49)
        {
        Distance_Since_Clear.Data_Length ++;
        }
        
        else
        {
            cout << "Broken Phython Script" << endl;
        }

        // Increment count to move to the next index in your array
        count++;

        // You may want to check if count exceeds the array size to avoid overflow
        if (count > n) {
            cout << "First Size exceeded. More entries than parameter n. Increase the size or handle accordingly." << endl;
            break;
        }
    }
    
    file.close();

    cout << Engine_Speed.Data_Length << endl;
    cout << Vehicle_Speed.Data_Length << endl;
    cout << ECT.Data_Length << endl;
    cout << Fuel_Percent.Data_Length << endl;
    cout << Distance_Since_Clear.Data_Length << endl;
    cout << count << endl;

    
    
    
    Engine_Speed.timestamp = (float *) calloc(Engine_Speed.Data_Length, sizeof(float));
    Engine_Speed.PID = (int *) calloc(Engine_Speed.Data_Length, sizeof(int)); // raster scan      
    Engine_Speed.Data = (float *) calloc(Engine_Speed.Data_Length, sizeof(float));
    if (Engine_Speed.timestamp == NULL) {printf("error");}
    if (Engine_Speed.PID == NULL) {printf("error");}
    if (Engine_Speed.Data == NULL) {printf("error");}


    Vehicle_Speed.timestamp = (float *) calloc(Vehicle_Speed.Data_Length, sizeof(float));
    Vehicle_Speed.PID = (int *) calloc(Vehicle_Speed.Data_Length, sizeof(int)); // raster scan      
    Vehicle_Speed.Data = (float *) calloc(Vehicle_Speed.Data_Length, sizeof(float));
    if (Vehicle_Speed.timestamp == NULL) {printf("error");}
    if (Vehicle_Speed.PID == NULL) {printf("error");}
    if (Vehicle_Speed.Data == NULL) {printf("error");}


    ECT.timestamp = (float *) calloc(ECT.Data_Length, sizeof(float));
    ECT.PID = (int *) calloc(ECT.Data_Length, sizeof(int)); // raster scan      
    ECT.Data = (float *) calloc(ECT.Data_Length, sizeof(float));
    if (ECT.timestamp == NULL) {printf("error");}
    if (ECT.PID == NULL) {printf("error");}
    if (ECT.Data == NULL) {printf("error");}


    Fuel_Percent.timestamp = (float *) calloc(Fuel_Percent.Data_Length, sizeof(float));
    Fuel_Percent.PID = (int *) calloc(Fuel_Percent.Data_Length, sizeof(int)); // raster scan      
    Fuel_Percent.Data = (float *) calloc(Fuel_Percent.Data_Length, sizeof(float));
    if (Fuel_Percent.timestamp == NULL) {printf("error");}
    if (Fuel_Percent.PID == NULL) {printf("error");}
    if (Fuel_Percent.Data == NULL) {printf("error");}


    Distance_Since_Clear.timestamp = (float *) calloc(Distance_Since_Clear.Data_Length, sizeof(float));
    Distance_Since_Clear.PID = (int *) calloc(Distance_Since_Clear.Data_Length, sizeof(int)); // raster scan      
    Distance_Since_Clear.Data = (float *) calloc(Distance_Since_Clear.Data_Length, sizeof(float));
    if (Distance_Since_Clear.timestamp == NULL) {printf("error");}
    if (Distance_Since_Clear.PID == NULL) {printf("error");}
    if (Distance_Since_Clear.Data == NULL) {printf("error");}
    
  
    count = 0;
   
     ifstream file1(filename);

    if (!file1.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    

    


    while (file1 >> TEMP_DATA_Input.timestamp >> TEMP_DATA_Input.PID >> TEMP_DATA_Input.Data) {
        // Assuming your struct has a public array and you want to store each entry

        if (TEMP_DATA_Input.PID == 12)
        {
        Engine_Speed.timestamp[count1] = TEMP_DATA_Input.timestamp;
        Engine_Speed.PID[count1] = TEMP_DATA_Input.PID;
        Engine_Speed.Data[count1] = TEMP_DATA_Input.Data;
        count1 ++;
        }

        else if (TEMP_DATA_Input.PID == 13)
        {
        Vehicle_Speed.timestamp[count2] = TEMP_DATA_Input.timestamp;
        Vehicle_Speed.PID[count2] = TEMP_DATA_Input.PID;
        Vehicle_Speed.Data[count2] = TEMP_DATA_Input.Data;
        count2 ++;
        }
        
        else if (TEMP_DATA_Input.PID == 05)
        {
        ECT.timestamp[count3] = TEMP_DATA_Input.timestamp;
        ECT.PID[count3] = TEMP_DATA_Input.PID;
        ECT.Data[count3] = TEMP_DATA_Input.Data;
        count3 ++;
        }

        else if (TEMP_DATA_Input.PID == 47)
        {
        Fuel_Percent.timestamp[count4] = TEMP_DATA_Input.timestamp;
        Fuel_Percent.PID[count4] = TEMP_DATA_Input.PID;
        Fuel_Percent.Data[count4] = TEMP_DATA_Input.Data;
        count4 ++;
        }

         else if (TEMP_DATA_Input.PID == 49)
        {
        Distance_Since_Clear.timestamp[count5] = TEMP_DATA_Input.timestamp;
        Distance_Since_Clear.PID[count5] = TEMP_DATA_Input.PID;
        Distance_Since_Clear.Data[count5] = TEMP_DATA_Input.Data;
        count5 ++;
        }
        
        else
        {
            cout << "Broken Phython Script" << endl;
        }

        // Increment count to move to the next index in your array
        count++;

        // You may want to check if count exceeds the array size to avoid overflow
        if (count > n) {
            cout << "Size exceeded. More entries than parameter n. Increase the size or handle accordingly." << endl;
            break;
        }
    }
    
    file1.close();
    
    //Create 2-D array of data vectors 
    //int Data_set_lengths[5] = {Engine_Speed.Data_Length , Vehicle_Speed.Data_Length, ECT.Data_Length, Fuel_Percent.Data_Length, Distance_Since_Clear.Data_Length};
    int number_of_rows = 5;
    int number_of_cols = Engine_Speed.Data_Length;
    
    float** A = (float**) calloc(number_of_rows, sizeof(float*));

    for(int i = 0; i < number_of_rows; ++i)
    {
        A[i] = (float*)calloc(5,sizeof(float));
    }

    A[0] = Engine_Speed.Data;
    A[1] = Vehicle_Speed.Data; 
    A[2] = ECT.Data; 
    A[3] = Fuel_Percent.Data; 
    A[4] = Distance_Since_Clear.Data;  
    

    float max[5];
    float min[5];

    gettimeofday (&start, NULL);
    //Max Values and Min Values
    tbb::parallel_for(int(0), int(number_of_rows*2), [&] (int i)
    {
        if(i < 5) {max[i] = getmax_tbb(A[i] , number_of_cols);}
        else if(i < 10) {min[i-5] = getmin_tbb(A[i-5] , number_of_cols);}
    });
    
    

    //Now let's get the histogram for engine speed
    //bins will be 0-499.9999, 500-1000, 1000-1500, 1500-2000, 2000-2500, 2500-3000, 3000-3500, 3500-4000, 4000-4500, 4500-5000, 5000-5500, 5500-6000, 6000-6500, 6500-7000
    //therefoe the bin_size = 500
  
    
    //Now let's get the histogram for vehicle speed
    

    gettimeofday (&end, NULL);
    for(int i = 0; i < number_of_rows; ++i)
    {
        free(A[i]);
    }
    free(A);
    int engine_speed_max_index = max[0];
    int vehicle_speed_max_index = max[1];
    int fuel_percent_max_index = max[2];
    int ECT_max_index = max[3];
    int Distance_Since_Clear_max_index = max[4];
    cout << "----------------------Max Values-----------------------------" << endl;
    cout << "Max Engine Speed: " << Engine_Speed.Data[engine_speed_max_index] << "rpm at " << Engine_Speed.timestamp[engine_speed_max_index] 
    << "s" << endl;
    cout << "Max Vehicle Speed: " << Vehicle_Speed.Data[vehicle_speed_max_index] << "kph at " << Vehicle_Speed.timestamp[vehicle_speed_max_index]
    << "s" << endl;
    cout << "Max Fuel Percentage: " << Fuel_Percent.Data[fuel_percent_max_index] << "\% at " << Fuel_Percent.timestamp[fuel_percent_max_index] 
    << "s" << endl;
    cout << "Max ECT: " << ECT.Data[ECT_max_index] << " Degrees Celsius at " << ECT.timestamp[ECT_max_index] 
    << "s" << endl;
    cout << "Max Distance Travelled: " << Distance_Since_Clear.Data[Distance_Since_Clear_max_index] << "km at " << Distance_Since_Clear.timestamp[Distance_Since_Clear_max_index] 
    << "s" << endl;
    cout << "-------------------------------------------------------------" << endl;
    int engine_speed_min_index = min[0];
    int vehicle_speed_min_index = min[1];
    int fuel_percent_min_index = min[2];
    int ECT_min_index = min[3];
    int Distance_Since_Clear_min_index = min[4];
    cout << "----------------------Min Values-----------------------------" << endl;
    cout << "Min Engine Speed: " << Engine_Speed.Data[engine_speed_min_index] << "rpm at " << Engine_Speed.timestamp[engine_speed_min_index] 
    << "s" << endl;
    cout << "Min Vehicle Speed: " << Vehicle_Speed.Data[vehicle_speed_min_index] << "kph at " << Vehicle_Speed.timestamp[vehicle_speed_min_index]
    << "s" << endl;
    cout << "Min Fuel Percentage: " << Fuel_Percent.Data[fuel_percent_min_index] << "\% at " << Fuel_Percent.timestamp[fuel_percent_min_index] 
    << "s" << endl;
    cout << "Min ECT: " << ECT.Data[ECT_min_index] << " Degrees Celsius at " << ECT.timestamp[ECT_min_index] 
    << "s" << endl;
    cout << "Min Distance Travelled: " << Distance_Since_Clear.Data[Distance_Since_Clear_min_index] << "km at " << Distance_Since_Clear.timestamp[Distance_Since_Clear_min_index] 
    << "s" << endl;
    cout << "-------------------------------------------------------------" << endl;
    
    cout << "----------------------Engine Speed Histogram Values-----------------------------" << endl;
    cout << "Engine Speed Histogram (Range 0-8000 RPM): Bin Size " << histogram_engine_speed_binsize << endl;
    for (int i = 0; i < 8000/histogram_engine_speed_binsize; i++)
    {
    cout << "Bin Number: " << i << "   Range: " << i*histogram_engine_speed_binsize << "  -  " << (i*histogram_engine_speed_binsize) + histogram_engine_speed_binsize << "   Value:  " << histogram_engine_speed[i] << endl;
    }
    cout << "-------------------------------------------------------------" << endl;


    cout << "----------------------Vehicle Speed Histogram Values-----------------------------" << endl;
    cout << "Engine Speed Histogram (Range 0-160 km/hr): Bin Size " << histogram_vehicle_speed_binsize << endl;
    for (int i = 0; i < 160/histogram_vehicle_speed_binsize; i++)
    {
    cout << "Bin Number: " << i << "   Range: " << i*histogram_vehicle_speed_binsize << "  -  " << (i*histogram_vehicle_speed_binsize) + histogram_vehicle_speed_binsize << "   Value:  " << histogram_vehicle_speed[i] << endl;
    }
    cout << "-------------------------------------------------------------" << endl;


    size_t result;
    //write Engine Speed Histogram to .bof file
    //*************************************
    file_o1 = fopen (out_file1,"wb");
       if (file_o1 == NULL) return -1;// check that the file was actually opened
       
       result = fwrite (histogram_engine_speed, sizeof(int), 8000/histogram_engine_speed_binsize, file_o1); // each element (pixel) is of size int (4 bytes)
                   
       printf ("Output binary file (engine speed): # of elements written = %d\n", result); // Total # of elements successfully read
       fclose (file_o1);


    //write Vehicle Speed Histogram to .bof file
    //*************************************
    file_o2 = fopen (out_file2,"wb");
       if (file_o2 == NULL) return -1;// check that the file was actually opened
       
       result = fwrite (histogram_vehicle_speed, sizeof(int), 160/histogram_vehicle_speed_binsize, file_o2); // each element (pixel) is of size int (4 bytes)
                   
       printf ("Output binary file (vehicle): # of elements written = %d\n", result); // Total # of elements successfully read
       fclose (file_o2);		


    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
     // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time: %ld us\n", t_us);

    //Do something with the data, like printing it for verification
    // for (int i = 0; i < Engine_Speed.Data_Length; i++) {
    //    cout << "Timestamp: " << Engine_Speed.timestamp[i] << ", Identifier: " << Engine_Speed.PID[i]  << ", Data: " << Engine_Speed.Data[i]  << endl;
    // }

    // for (int i = 0; i < Vehicle_Speed.Data_Length; i++) {
    //    cout << "Timestamp: " << Vehicle_Speed.timestamp[i] << ", Identifier: " << Vehicle_Speed.PID[i]  << ", Data: " << Vehicle_Speed.Data[i]  << endl;
    // }

    // for (int i = 0; i < ECT.Data_Length; i++) {
    //    cout << "Timestamp: " << ECT.timestamp[i] << ", Identifier: " << ECT.PID[i]  << ", Data: " << ECT.Data[i]  << endl;
    // }

    // for (int i = 0; i < Fuel_Percent.Data_Length; i++) {
    //    cout << "Timestamp: " << Fuel_Percent.timestamp[i] << ", Identifier: " << Fuel_Percent.PID[i]  << ", Data: " << Fuel_Percent.Data[i]  << endl;
    // }
    

    return 0;
}

    
   

