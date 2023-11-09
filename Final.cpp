using namespace std; 
#include <iostream>
#include <fstream>
#include <string>

typedef struct 
{
    float *timestamp;
    int *PID;
    int *Data;
    int Data_Length;
} DATA1;

typedef struct 
{
    float timestamp;
    int PID;
    int Data;
}   tmp_data;

int main() {
    int n = 10000;

    DATA1 Engine_Speed;
    Engine_Speed.Data_Length = n/4;
    Engine_Speed.timestamp = (float *) calloc(Engine_Speed.Data_Length, sizeof(float));
    Engine_Speed.PID = (int *) calloc(Engine_Speed.Data_Length, sizeof(int)); // raster scan      
    Engine_Speed.Data = (int *) calloc(Engine_Speed.Data_Length, sizeof(int));
    if (Engine_Speed.timestamp == NULL) {printf("error");}
    if (Engine_Speed.PID == NULL) {printf("error");}
    if (Engine_Speed.Data == NULL) {printf("error");}

    DATA1 Vehicle_Speed;
    Vehicle_Speed.Data_Length = n/4;
    Vehicle_Speed.timestamp = (float *) calloc(Vehicle_Speed.Data_Length, sizeof(float));
    Vehicle_Speed.PID = (int *) calloc(Vehicle_Speed.Data_Length, sizeof(int)); // raster scan      
    Vehicle_Speed.Data = (int *) calloc(Vehicle_Speed.Data_Length, sizeof(int));
    if (Vehicle_Speed.timestamp == NULL) {printf("error");}
    if (Vehicle_Speed.PID == NULL) {printf("error");}
    if (Vehicle_Speed.Data == NULL) {printf("error");}

    DATA1 ECT;
    ECT.Data_Length = n/4;
    ECT.timestamp = (float *) calloc(ECT.Data_Length, sizeof(float));
    ECT.PID = (int *) calloc(ECT.Data_Length, sizeof(int)); // raster scan      
    ECT.Data = (int *) calloc(ECT.Data_Length, sizeof(int));
    if (ECT.timestamp == NULL) {printf("error");}
    if (ECT.PID == NULL) {printf("error");}
    if (ECT.Data == NULL) {printf("error");}

    DATA1 Fuel_Percent;
    Fuel_Percent.Data_Length = n/4;
    Fuel_Percent.timestamp = (float *) calloc(Fuel_Percent.Data_Length, sizeof(float));
    Fuel_Percent.PID = (int *) calloc(Fuel_Percent.Data_Length, sizeof(int)); // raster scan      
    Fuel_Percent.Data = (int *) calloc(Fuel_Percent.Data_Length, sizeof(int));
    if (Fuel_Percent.timestamp == NULL) {printf("error");}
    if (Fuel_Percent.PID == NULL) {printf("error");}
    if (Fuel_Percent.Data == NULL) {printf("error");}

    tmp_data TEMP_DATA_Input;
  

    const char* filename = "data.txt";  // Change this to your file name

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    
    int count = 0;

    while (file >> TEMP_DATA_Input.timestamp >> TEMP_DATA_Input.PID >> TEMP_DATA_Input.Data) {
        // Assuming your struct has a public array and you want to store each entry

        if (TEMP_DATA_Input.PID == 12)
        {
        Engine_Speed.timestamp[count] = TEMP_DATA_Input.timestamp;
        Engine_Speed.PID[count] = TEMP_DATA_Input.PID;
        Engine_Speed.Data[count] = TEMP_DATA_Input.Data;
        }

        else if (TEMP_DATA_Input.PID == 13)
        {
        Vehicle_Speed.timestamp[count] = TEMP_DATA_Input.timestamp;
        Vehicle_Speed.PID[count] = TEMP_DATA_Input.PID;
        Vehicle_Speed.Data[count] = TEMP_DATA_Input.Data;
        }
        
        else if (TEMP_DATA_Input.PID == 103)
        {
        ECT.timestamp[count] = TEMP_DATA_Input.timestamp;
        ECT.PID[count] = TEMP_DATA_Input.PID;
        ECT.Data[count] = TEMP_DATA_Input.Data;
        }

        else if (TEMP_DATA_Input.PID == 47)
        {
        Fuel_Percent.timestamp[count] = TEMP_DATA_Input.timestamp;
        Fuel_Percent.PID[count] = TEMP_DATA_Input.PID;
        Fuel_Percent.Data[count] = TEMP_DATA_Input.Data;
        }
        
        else
        {
            cout << "Broken Phython Script" << endl;
        }

        // Increment count to move to the next index in your array
        count++;

        // You may want to check if count exceeds the array size to avoid overflow
        if (count >= n) {
            cout << "Size exceeded. More entries than parameter n. Increase the size or handle accordingly." << endl;
            break;
        }
    }
    
    file.close();

    // Now Engine_Speed contains your data

    // Do something with the data, like printing it for verification
    for (int i = 0; i < Vehicle_Speed.Data_Length; i++) {
        cout << "Timestamp: " << Vehicle_Speed.timestamp[i] << ", Identifier: " << Vehicle_Speed.PID[i]  << ", Data: " << Vehicle_Speed.Data[i]  << endl;
    }

    return 0;
}

    
   

