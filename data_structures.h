// data_structures.h

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

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
} tmp_data;

// Function to duplicate elements in an array
void duplicateArray(DATA1 original, int n_multiplier) {

    for (int i = 0; i < (original.Data_Length/n_multiplier); i++) 
    {   for (int j = 1; j < n_multiplier; j++)
        {
         original.timestamp[(original.Data_Length/n_multiplier*j)+i] =  original.timestamp[i];
         original.PID[(original.Data_Length/n_multiplier*j)+i] =  original.PID[i];
         original.Data[(original.Data_Length/n_multiplier*j)+i] =  original.Data[i];
        }     
    }
}

#endif // DATA_STRUCTURES_H
