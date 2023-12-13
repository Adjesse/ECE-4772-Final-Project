#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include "data_structures.h"
#include "seq_functions.h"
#include <iomanip>


using namespace std;


int main(int argc, char **argv) {


    int n = 305000;
    int n_multiplier;

    int engine_speed_h_binsize = 500;
    int engine_speed_h_maxvalue = 8000;
    int vehicle_speed_h_binsize = 10;
    int vehicle_speed_h_maxvalue = 160;

    if (argc != 2) 
    {
       printf ("Warning: Usage: %s n\n", argv[0]); 
       printf ("Using n_multiplier = 1 as default\n");
       n_multiplier = 1;
    }
    else 
    {
        n_multiplier = atoi(argv[1]);  
    }

    FILE *file_o1;
    FILE *file_o2;
    char *out_file1 = "engine_speed_histogram.bof";
    char *out_file2 = "vehicle_speed_histogram.bof";

    DATA1 Engine_Speed;
    DATA1 Vehicle_Speed;
    DATA1 ECT;
    DATA1 Fuel_Percent;
    DATA1 Distance_Since_Clear;
    

    int engine_speed_h[engine_speed_h_maxvalue/engine_speed_h_binsize];
    int vehicle_speed_h[vehicle_speed_h_maxvalue/vehicle_speed_h_binsize];

    for(int i = 0; i <8000/engine_speed_h_binsize; i++)
    {
        engine_speed_h[i] = 0;
    }

    for(int i = 0; i <160/vehicle_speed_h_binsize; i++)
    {
        vehicle_speed_h[i] = 0;
    }


    
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

    
    Engine_Speed.Data_Length = Engine_Speed.Data_Length*n_multiplier;
    Vehicle_Speed.Data_Length = Vehicle_Speed.Data_Length*n_multiplier;
    ECT.Data_Length = ECT.Data_Length*n_multiplier;
    Fuel_Percent.Data_Length = Fuel_Percent.Data_Length*n_multiplier;
    Distance_Since_Clear.Data_Length = Distance_Since_Clear.Data_Length*n_multiplier;

    std::cout << "Engine Speed Data Length =      " << std::setw(5) << Engine_Speed.Data_Length << std::endl;
    std::cout << "Vehicle Speed Data Length =     " << std::setw(5) << Vehicle_Speed.Data_Length << std::endl;
    std::cout << "ECT Data Length =               " << std::setw(5) << ECT.Data_Length << std::endl;
    std::cout << "Fuel Percent Data Length =      " << std::setw(5) << Fuel_Percent.Data_Length << std::endl;
    std::cout << "Distance Traveled Data Length = " << std::setw(5) << Distance_Since_Clear.Data_Length << std::endl;
    std::cout << "Total Data Set Length =         " << std::setw(5) << count * n_multiplier << std::endl;

    
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
    

    float *acc;
    int *acc_h;
    acc = (float *) calloc(Vehicle_Speed.Data_Length, sizeof(float));
    acc_h = (int *) calloc(20/1, sizeof(int));
  
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

    //expand the vector by themselves n_multiplier times
    duplicateArray(Engine_Speed, n_multiplier);
    duplicateArray(Vehicle_Speed, n_multiplier);
    duplicateArray(ECT, n_multiplier);
    duplicateArray(Fuel_Percent, n_multiplier);
    duplicateArray(Distance_Since_Clear, n_multiplier);


    // Now Engine_Speed contains your data
    gettimeofday (&start, NULL);
    //Max Values
    int engine_speed_max_index = findMax(Engine_Speed.Data,Engine_Speed.Data_Length);
    int vehicle_speed_max_index = findMax(Vehicle_Speed.Data,Vehicle_Speed.Data_Length);
    int fuel_percent_max_index = findMax(Fuel_Percent.Data,Fuel_Percent.Data_Length);
    int ECT_max_index = findMax(ECT.Data,ECT.Data_Length);
    int Distance_Since_Clear_max_index = findMax(Distance_Since_Clear.Data,Distance_Since_Clear.Data_Length);
    
    //Min Values
    int engine_speed_min_index = findMin(Engine_Speed.Data,Engine_Speed.Data_Length);
    int vehicle_speed_min_index = findMin(Vehicle_Speed.Data,Vehicle_Speed.Data_Length);
    int fuel_percent_min_index = findMin(Fuel_Percent.Data,Fuel_Percent.Data_Length);
    int ECT_min_index = findMin(ECT.Data,ECT.Data_Length);
    int Distance_Since_Clear_min_index = findMin(Distance_Since_Clear.Data,Distance_Since_Clear.Data_Length);
    
    //AVG Values
    float engine_speed_avg = findAvg(Engine_Speed.Data,Engine_Speed.Data_Length);
    float vehicle_speed_avg = findAvg(Vehicle_Speed.Data,Vehicle_Speed.Data_Length);
    float fuel_percent_avg = findAvg(Fuel_Percent.Data,Fuel_Percent.Data_Length);
    float ECT_avg = findAvg(ECT.Data,ECT.Data_Length);
    float Distance_Since_Clear_avg = findAvg(Distance_Since_Clear.Data,Distance_Since_Clear.Data_Length);

    //Now let's get the histogram for engine speed
    //bins will be 0-499.9999, 500-1000, 1000-1500, 1500-2000, 2000-2500, 2500-3000, 3000-3500, 3500-4000, 4000-4500, 4500-5000, 5000-5500, 5500-6000, 6000-6500, 6500-7000
    //therefoe the bin_size = 500
    CreateHistogram(Engine_Speed.Data, engine_speed_h, engine_speed_h_binsize, Engine_Speed.Data_Length);
    
    //Now let's get the histogram for vehicle speed
    CreateHistogram(Vehicle_Speed.Data, vehicle_speed_h, vehicle_speed_h_binsize, Vehicle_Speed.Data_Length);
    
    for(int i = 1; i < Vehicle_Speed.Data_Length; i++)
    {
        acc[i] = (Vehicle_Speed.Data[i] -  Vehicle_Speed.Data[i-1]) / ((Vehicle_Speed.timestamp[i] - Vehicle_Speed.timestamp[i-1])*3.6);
        cout << acc[i] << endl;
    }
     for (int i = 0; i < 20/1; i++)
    {
    cout << "Bin Number: " << i << "   Range: " << (i*1)-10 << "  -  " << (i*1) + 1 - 10<< "   Value:  " << acc_h[i] << endl;
    }
    cout << "-------------------------------------------------------------" << endl;
    CreateHistogram_w_negatives(acc , acc_h , 1 , Vehicle_Speed.Data_Length,10);

    gettimeofday (&end, NULL);


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
    
    cout << "----------------------Avg Values-----------------------------" << endl;
    cout << "Avg Engine Speed: " << engine_speed_avg << "rpm  "  << endl;
    cout << "Avg Vehicle Speed: " << vehicle_speed_avg << "kph  " << endl;
    cout << "Avg Fuel Percentage: " << fuel_percent_avg<< "\% " << endl;
    cout << "Avg ECT: " << ECT_avg<< " Degrees Celsius" << endl;
    cout << "Avg Distance Travelled: " << Distance_Since_Clear_avg << "km" << endl;
    cout << "-------------------------------------------------------------" << endl;

    cout << "----------------------Engine Speed Histogram Values-----------------------------" << endl;
    cout << "Engine Speed Histogram (Range 0-8000 RPM): Bin Size " << engine_speed_h_binsize << endl;
    for (int i = 0; i < 8000/engine_speed_h_binsize; i++)
    {
    cout << "Bin Number: " << i << "   Range: " << i*engine_speed_h_binsize << "  -  " << (i*engine_speed_h_binsize) + engine_speed_h_binsize << "   Value:  " << engine_speed_h[i] << endl;
    }
    cout << "-------------------------------------------------------------" << endl;


    cout << "----------------------Vehicle Speed Histogram Values-----------------------------" << endl;
    cout << "Engine Speed Histogram (Range 0-160 km/hr): Bin Size " << vehicle_speed_h_binsize << endl;
    for (int i = 0; i < 160/vehicle_speed_h_binsize; i++)
    {
    cout << "Bin Number: " << i << "   Range: " << i*vehicle_speed_h_binsize << "  -  " << (i*vehicle_speed_h_binsize) + vehicle_speed_h_binsize << "   Value:  " << vehicle_speed_h[i] << endl;
    }
    cout << "-------------------------------------------------------------" << endl;

    cout << "----------------------Acc Speed Histogram Values-----------------------------" << endl;
    cout << "Acc Histogram (Range 0-10 m/s^2): Bin Size " << 1 << endl;
    for (int i = 0; i < 20/1; i++)
    {
    cout << "Bin Number: " << i << "   Range: " << (i*1)-10 << "  -  " << (i*1) + 1 - 10<< "   Value:  " << acc_h[i] << endl;
    }
    cout << "-------------------------------------------------------------" << endl;


    size_t result;
    //write Engine Speed Histogram to .bof file
    //*************************************
    file_o1 = fopen (out_file1,"wb");
       if (file_o1 == NULL) return -1;// check that the file was actually opened
       
       result = fwrite (engine_speed_h, sizeof(int), 8000/engine_speed_h_binsize, file_o1); // each element (pixel) is of size int (4 bytes)
                   
       printf ("Output binary file (engine speed): # of elements written = %d\n", result); // Total # of elements successfully read
       fclose (file_o1);


    //write Vehicle Speed Histogram to .bof file
    //*************************************
    file_o2 = fopen (out_file2,"wb");
       if (file_o2 == NULL) return -1;// check that the file was actually opened
       
       result = fwrite (vehicle_speed_h, sizeof(int), 160/vehicle_speed_h_binsize, file_o2); // each element (pixel) is of size int (4 bytes)
                   
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

    
   

