#ifndef PIPLINE_H
#define PIPLINE_H

#pragma once  // Include guard to prevent multiple inclusion
#include <iostream>
#include <tbb/tbb.h>
#include "seq_functions.h"


using namespace std;
using namespace tbb;

class Vehicle_Speed {
public:
    int Data_Length;
    int vs_data_length = 100;
    float* Data;
    float* timestamp;
    float* acceleration;
};


class input{
    int Data_Length;          //inputs from main
    float* Data;
    float* timestamp;

    float* acceleration;      //output variables

    mutable int i;           //iterate through arrays
public:
    //assign input values 
    input(float* Data_i, float* timestamp_i, int Data_Length_i, float* acceleration_i): 
    Data_Length(Data_Length_i), timestamp(timestamp_i), Data(Data_i), acceleration(acceleration_i), i(0) {}

    Vehicle_Speed operator()(flow_control& fc) const {
        Vehicle_Speed VSS;
        if(i < Data_Length/VSS.vs_data_length)
        {
            VSS.Data_Length = Data_Length;
            VSS.Data = (Data+100*i);
            VSS.timestamp = (timestamp+100*i);
            VSS.acceleration = (acceleration+100*i);
            i++;
            return VSS;  
        }
        else 
        {   
            //cout << "Stage 1 Complete" << endl;
            fc.stop();
            return VSS;
        }   
    }
};


class CalculateAcceleration {
public:
    float* operator() (Vehicle_Speed input) const {
        float* acceleration = input.acceleration;
        for (int i = 0; i < input.vs_data_length; ++i) {
            acceleration[i] = (input.Data[i+1] - input.Data[i]) / ((input.timestamp[i+1] - input.timestamp[i]) * 3.6);
        }
        return acceleration;
    }
};

class MIN_AND_MAX {
public:
float max; 
float min;
};

class MinMax{
    int input_data_length;
public: 
    MinMax(int Data_Length_i): input_data_length(Data_Length_i){}
    MIN_AND_MAX operator() (float* input_acc) const {
        
        MIN_AND_MAX out;
        //cout << "Stage 3" << endl; 
        int temp1; 
        int temp2;
        temp1 = findMax(input_acc,100);
        temp2 = findMin(input_acc,100);
        out.max = input_acc[temp1];
        out.min = input_acc[temp2];

        return out;
    }
};

class output{
    int Data_Length;          //inputs from main
    float* Data;
    float* timestamp;

    float* acceleration;      //output variables
    mutable int i;           //iterate through a

    int *out;
public: 

output(int *out_i): out(out_i) {}

void operator()(MIN_AND_MAX max_min_input) const {
    float max = max_min_input.max;
    float min = max_min_input.min;
            //cout << "Stage 4" << endl; 


    if (max > 2.7)
    {
        *(out) = *(out) + 1;
    }
    else if(min < -5.4)
    {
        *(out+1) = *(out+1) + 1;
    }
    else{
        *(out+2) = *(out+2) + 1;
    }

    }
};

// Define the TBB pipeline
void RunPipeline(float* data, float* timestamp, int datalength, float* acceleration, int* piplineoutput) {
    int ntoken = 16;
    tbb::parallel_pipeline(ntoken,
        tbb::make_filter<void, Vehicle_Speed>(tbb::filter_mode::serial_in_order, input(data,timestamp,datalength,acceleration) )
      & tbb::make_filter<Vehicle_Speed, float*>(tbb::filter_mode::parallel, CalculateAcceleration() ) 
      & tbb::make_filter<float*, MIN_AND_MAX>(tbb::filter_mode::serial_in_order, MinMax(datalength) )
      & tbb::make_filter<MIN_AND_MAX, void>(tbb::filter_mode::serial_in_order,output(piplineoutput) ));

};




#endif