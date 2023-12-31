#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <tbb/tbb.h>
#include "data_structures.h"
#include "computation_classes.h"
#include <iomanip>
#include "pipline.h"

using namespace std;
using namespace tbb; 




int main(int argc, char **argv) {

//--------------------------------Common Setup--------------------------------    
    int n = 305000;
    int n_multiplier;

    int engine_speed_h_binsize = 500;
    int engine_speed_h_maxvalue = 8000;
    int vehicle_speed_h_binsize = 10;
    int vehicle_speed_h_maxvalue = 160;
    int acceleration_h_binsize = 1;
    int acceleration_h_maxvalue = 20;


    int nt; //nt = number of partial histograms computed for the 61,000 datapoints
    // Reading Input arguments
    //   argc: # of arguments. 
    //   argv: array of strings holding the arguments. argv[0]: ./mysequence, argv[1]: n
    if (argc != 3) 
    {
       printf ("Warning: Usage: %s n\n", argv[0]); 
       printf ("Using n = 10 as default\n");
       nt = 4;
       printf ("Using n_multiplier = 1 as default\n");
       n_multiplier = 1;
    }
    else 
    {
        nt = atoi(argv[2]);
        n_multiplier = atoi(argv[1]);  
    }  
    
    FILE *file_o1;
    FILE *file_o2;
    FILE *file_o3;
    char *out_file1 = "engine_speed_histogram.bof";
    char *out_file2 = "vehicle_speed_histogram.bof";
    char *out_file3 = "acceleration_histogram.bof";


    DATA1 Engine_Speed;
    DATA1 Vehicle_Speed;
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


//

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
    
//--------------------------------End Common Setup----------------------------



    
    float *acceleration;
    acceleration = (float *) calloc(Vehicle_Speed.Data_Length, sizeof(float));

    int pipeline_result[3] = {0,0,0};
    //variable setup and dynamic memory allocation for the min, max, and avg
    //Create 2-D array of data vectors 
    //int Data_set_lengths[5] = {Engine_Speed.Data_Length , Vehicle_Speed.Data_Length, ECT.Data_Length, Fuel_Percent.Data_Length, Distance_Since_Clear.Data_Length};
    int number_of_rows = 5;
    int number_of_cols[5] = {Engine_Speed.Data_Length, Vehicle_Speed.Data_Length, ECT.Data_Length, Fuel_Percent.Data_Length, Distance_Since_Clear.Data_Length}; 
    
    float** A = (float **) calloc(number_of_rows, sizeof(float*));

    for(int i = 0; i < number_of_rows; ++i)
    {
        A[i] = (float *)calloc(number_of_cols[i],sizeof(float));
    }

    A[0] = Engine_Speed.Data;
    A[1] = Vehicle_Speed.Data; 
    A[2] = ECT.Data; 
    A[3] = Fuel_Percent.Data; 
    A[4] = Distance_Since_Clear.Data;  
    
    float max[5];
    float min[5];
    float avg[5];


    //variable setup and dynamic variable allocation for the histograms for engine speed and vehicle speed

    int **engine_speed_hp;
    int *engine_speed_h;
    engine_speed_hp = (int **) calloc ((nt), sizeof(int*));
    for(int i = 0; i < nt; i++)
    {
        engine_speed_hp[i] = (int*)calloc((engine_speed_h_maxvalue/engine_speed_h_binsize),sizeof(int));
    }
    engine_speed_h = (int *) calloc((engine_speed_h_maxvalue/engine_speed_h_binsize), sizeof(int));
    // for(int i = 0; i <(engine_speed_h_maxvalue/engine_speed_h_binsize); i++)
    // {
    //     engine_speed_h[i] = 0;
    // }

    int **vehicle_speed_hp;
    int *vehicle_speed_h;
    vehicle_speed_hp = (int **) calloc ((nt), sizeof(int*));
    for(int i = 0; i < nt; i++)
    {
        vehicle_speed_hp[i] = (int*)calloc((vehicle_speed_h_maxvalue/vehicle_speed_h_binsize),sizeof(int));
    }
    vehicle_speed_h = (int *) calloc((vehicle_speed_h_maxvalue/vehicle_speed_h_binsize), sizeof(int));
    // for(int i = 0; i <(vehicle_speed_h_maxvalue/vehicle_speed_h_binsize); i++)
    // {
    //     vehicle_speed_h[i] = 0;
    // }//

    int **acceleration_hp;
    int *acceleration_h;
    acceleration_hp = (int **) calloc ((nt), sizeof(int*));
    for(int i = 0; i < nt; i++)
    {
        acceleration_hp[i] = (int*)calloc((acceleration_h_maxvalue/acceleration_h_binsize),sizeof(int));
    }
    acceleration_h = (int *) calloc((acceleration_h_maxvalue/acceleration_h_binsize), sizeof(int));



    gettimeofday (&start, NULL);

    RunPipeline(Vehicle_Speed.Data, Vehicle_Speed.timestamp, Vehicle_Speed.Data_Length, acceleration, pipeline_result);
          
           
    //Max Values and Min Values
    tbb::parallel_for(int(0), int(number_of_rows*3) + (3*nt), [&] (int i)
    {   
        if(i < 5) {max[i] = getmax_tbb(A[i] , number_of_cols[i]);}
        else if(i < 10) {min[i-5] = getmin_tbb(A[i-5] , number_of_cols[i-5]);}
        else if(i < 15) {avg[i-10] = ((accumulation_tbb(A[i-10] , number_of_cols[i-10]))/number_of_cols[i-10]);}
        else if(i < (15 + nt))    //for debugging nt = 4      ---> i = 15 to i = 18
        {
            engine_speed_hp[i-15] = CreatePartialHistogram (Engine_Speed.Data, (i-15), nt, Engine_Speed.Data_Length, engine_speed_h_binsize, engine_speed_h_maxvalue);
            //engine_speed_hp[0:3] = CreatePartialHistogram (Engine_Speed.Data, 0:3, 4, 61000, 500, 8000);           
        }
        else if(i < (15 + (2 * nt)))      //for debugging nt = 4    ---> i = 19 to i = 22
        {       
            vehicle_speed_hp[(i-(15+nt))] = CreatePartialHistogram (Vehicle_Speed.Data, (i-(15+nt)), nt, Vehicle_Speed.Data_Length, vehicle_speed_h_binsize, vehicle_speed_h_maxvalue);
            //vehicle_speed_hp[0:3] = CreatePartialHistogram (Vehicle_Speed.Data, 0:3, 4, 61000, 10, 160);     
        }
        else if(i < (15 + (3 * nt)))      //for debugging nt = 4    ---> i = 23 to i = 26
        {       
            acceleration_hp[(i-(15+(2*nt)))] = CreatePartialHistogram (acceleration, (i-(15+(2*nt))), nt, Vehicle_Speed.Data_Length, acceleration_h_binsize, acceleration_h_maxvalue);
            //vehicle_speed_hp[0:3] = CreatePartialHistogram (Vehicle_Speed.Data, 0:3, 4, 61000, 10, 160);     
        }

    });



    engine_speed_h = getsum_tbb(engine_speed_hp, nt, engine_speed_h_binsize, engine_speed_h_maxvalue);
    vehicle_speed_h = getsum_tbb(vehicle_speed_hp, nt, vehicle_speed_h_binsize, vehicle_speed_h_maxvalue);
    acceleration_h = getsum_tbb(acceleration_hp, nt, acceleration_h_binsize, acceleration_h_maxvalue);

//int main() {
//     Vehicle_Speed vehicleSpeed;  // Replace this with your actual instantiation
//     // Assuming vehicleSpeed is initialized with the required data


    


//     // Now 'acc' vector contains acceleration values
//     // Do something with the 'acc' vector if needed

//     return 0;
// }
   
////
    gettimeofday (&end, NULL);

    
    
    cout << "Hard Acceleration" << "  =  " << pipeline_result[0] << endl;
    cout << "Hard Braking" << "  =  " << pipeline_result[1] << endl;
    cout << "Cruising" << "  =  " << pipeline_result[2] << endl;
    

    // for(int i = 0; i < 40; i++)
    // {
    //     cout << "Acceleration  " << i << "  : " << acceleration[i] << endl;
    // }
    //Now let's get the histogram for engine speed
    //bins will be 0-499.9999, 500-1000, 1000-1500, 1500-2000, 2000-2500, 2500-3000, 3000-3500, 3500-4000, 4000-4500, 4500-5000, 5000-5500, 5500-6000, 6000-6500, 6500-7000
    //therefoe the bin_size = 500
   
    //Now let's get the histogram for vehicle speed////
    
    int engine_speed_max_index = max[0];
    int vehicle_speed_max_index = max[1];
    int ECT_max_index = max[2];
    int fuel_percent_max_index = max[3];
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
    int ECT_min_index = min[2];
    int fuel_percent_min_index = min[3];
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
   
    cout << "----------------------Avg Values-----------------------------" << endl;
    cout << "Avg Engine Speed: " << avg[0] << "rpm  "  << endl;
    cout << "Avg Vehicle Speed: " << avg[1] << "kph  " << endl;
    cout << "Avg Fuel Percentage: " << avg[3] << "\% " << endl;
    cout << "Avg ECT: " << avg[2] << " Degrees Celsius" << endl;
    cout << "Avg Distance Travelled: " << avg[4] << "km" << endl;
    cout << "-------------------------------------------------------------" << endl;
    
    cout << "----------------------Engine Speed Histogram Values-----------------------------" << endl;
    cout << "Engine Speed Histogram (Range 0-" << engine_speed_h_maxvalue << " RPM): Bin Size " << engine_speed_h_binsize << endl;
    for (int i = 0; i < engine_speed_h_maxvalue/engine_speed_h_binsize; i++)
    {
    cout << "Bin Number: " << i << "   Range: " << i*engine_speed_h_binsize << "  -  " << (i*engine_speed_h_binsize) + engine_speed_h_binsize << "   Value:  " << engine_speed_h[i] << endl;
    }
    cout << "-------------------------------------------------------------" << endl;


    cout << "----------------------Vehicle Speed Histogram Values-----------------------------" << endl;
    cout << "Engine Speed Histogram (Range 0-"<< vehicle_speed_h_maxvalue << " km/hr): Bin Size " << vehicle_speed_h_binsize << endl;
    for (int i = 0; i < vehicle_speed_h_maxvalue/vehicle_speed_h_binsize; i++)
    {
    cout << "Bin Number: " << i << "   Range: " << i*vehicle_speed_h_binsize << "  -  " << (i*vehicle_speed_h_binsize) + vehicle_speed_h_binsize << "   Value:  " << vehicle_speed_h[i] << endl;
    }
    cout << "-------------------------------------------------------------" << endl;

    cout << "----------------------Acceleration Histogram Values-----------------------------" << endl;
    cout << "Acceleration Histogram (Range " << 10 - acceleration_h_maxvalue << " to " << acceleration_h_maxvalue - 10 << " m/(s^2): Bin Size " << acceleration_h_binsize << endl;
    for (int i = 0; i < acceleration_h_maxvalue/acceleration_h_binsize; i++)
    {
    cout << "Bin Number: " << i << "   Range: " << (i*acceleration_h_binsize)-10 << "  -  " << (i*acceleration_h_binsize) + acceleration_h_binsize - 10 << "   Value:  " << acceleration_h[i] << endl;
    }
    cout << "-------------------------------------------------------------" << endl;


    size_t result;
    //write Engine Speed Histogram to .bof file
    //*************************************
    file_o1 = fopen (out_file1,"wb");
       if (file_o1 == NULL) return -1;// check that the file was actually opened
       
       result = fwrite (engine_speed_h, sizeof(int), engine_speed_h_maxvalue/engine_speed_h_binsize, file_o1); // each element (pixel) is of size int (4 bytes)
                   
       printf ("Output binary file (engine speed): # of elements written = %d\n", result); // Total # of elements successfully read
       fclose (file_o1);


    //write Vehicle Speed Histogram to .bof file
    //*************************************
    file_o2 = fopen (out_file2,"wb");
       if (file_o2 == NULL) return -1;// check that the file was actually opened
       
       result = fwrite (vehicle_speed_h, sizeof(int), vehicle_speed_h_maxvalue/vehicle_speed_h_binsize, file_o2); // each element (pixel) is of size int (4 bytes)
                   
       printf ("Output binary file (vehicle speed): # of elements written = %d\n", result); // Total # of elements successfully read
       fclose (file_o2);	

    //write Acceleration Histogram to .bof file
    //*************************************
    file_o3 = fopen (out_file3,"wb");
       if (file_o3 == NULL) return -1;// check that the file was actually opened
       
       result = fwrite (acceleration_h, sizeof(int), acceleration_h_maxvalue/acceleration_h_binsize, file_o3); // each element (pixel) is of size int (4 bytes)
                   
       printf ("Output binary file (acceleration): # of elements written = %d\n", result); // Total # of elements successfully read
       fclose (file_o3);			


    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
     // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time: %ld us\n", t_us);
    
    free(acceleration);

    for(int i = 0; i < nt; i++)
    {
        free(engine_speed_hp[i]);
    }
    free(engine_speed_hp);

    for(int i = 0; i < nt; i++)
    {
        free(acceleration_hp[i]);
    }
    free(acceleration_hp);

    for(int i = 0; i < nt; i++)
    {
        free(vehicle_speed_hp[i]);
    }
    free(vehicle_speed_hp);

    for(int i = 0; i < number_of_rows; i++)
    {
        free(A[i]);
    }
    free(A);

    free(engine_speed_h); free(vehicle_speed_h); free(acceleration_h);

    cout << "Done" << endl;
    return 0;
}

    
   

