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
    Engine_Speed.Data_Length = n;
    Engine_Speed.timestamp = (float *) calloc(Engine_Speed.Data_Length, sizeof(float));
    Engine_Speed.PID = (int *) calloc(Engine_Speed.Data_Length, sizeof(int)); // raster scan      
    Engine_Speed.Data = (int *) calloc(Engine_Speed.Data_Length, sizeof(int));
    if (Engine_Speed.timestamp == NULL) {printf("error");}
    if (Engine_Speed.PID == NULL) {printf("error");}
    if (Engine_Speed.Data == NULL) {printf("error");}

    DATA1 Vehicle_Speed;
    Engine_Speed.Data_Length = n;
    Vehicle_Speed.timestamp = (float *) calloc(Vehicle_Speed.Data_Length, sizeof(float));
    Vehicle_Speed.PID = (int *) calloc(Vehicle_Speed.Data_Length, sizeof(int)); // raster scan      
    Vehicle_Speed.Data = (int *) calloc(Vehicle_Speed.Data_Length, sizeof(int));
    if (Vehicle_Speed.timestamp == NULL) {printf("error");}
    if (Vehicle_Speed.PID == NULL) {printf("error");}
    if (Vehicle_Speed.Data == NULL) {printf("error");}

    tmp_data TEMP_DATA_Engine_Speed;
  

    const char* filename = "data.txt";  // Change this to your file name

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }
    
    int count = 0;

    while (file >> TEMP_DATA_Engine_Speed.timestamp >> TEMP_DATA_Engine_Speed.PID >> TEMP_DATA_Engine_Speed.Data) {
        // Assuming your struct has a public array and you want to store each entry
        Engine_Speed.timestamp[count] = TEMP_DATA_Engine_Speed.timestamp;
        Engine_Speed.PID[count] = TEMP_DATA_Engine_Speed.PID;
        Engine_Speed.Data[count] = TEMP_DATA_Engine_Speed.Data;

        // Increment count to move to the next index in your array
        count++;

        // You may want to check if count exceeds the array size to avoid overflow
        if (count >= Engine_Speed.Data_Length) {
            cerr << "Array size exceeded. Increase the size or handle accordingly." << endl;
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

    
   

