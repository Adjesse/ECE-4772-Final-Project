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

#endif // DATA_STRUCTURES_H
