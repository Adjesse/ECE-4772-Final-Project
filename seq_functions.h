// seq_functions.h

#ifndef SEQ_FUNCTIONS_H
#define SEQ_FUNCTIONS_H

int findMax(float *data, int datalength) {
    float index_of_max = 0;
    float max = data[0];
    for (int i = 0; i < datalength; i++) {
        if (data[i] > max) {
            max = data[i];
            index_of_max = i;
        }
    }
    return index_of_max;
}

int findMin(float *data, int datalength) {
    float index_of_min = 0;
    float min = data[0];
    for (int i = 0; i < datalength; i++) {
        if (data[i] < min) {
            min = data[i];
            index_of_min = i;
        }
    }
    return index_of_min;
}

float findAvg(float *data, int datalength) {
    float sum = 0;
    float avg = 0;
    for (int i = 0; i < datalength; i++) {
        sum = sum + data[i];
    }
    avg = sum / datalength;
    return avg;
}

void CreateHistogram(float *INPUT_DATA, int OUTPUT_DATA[], int bin_size, int data_length) {
    for (int i = 0; i < data_length; i++) {
        OUTPUT_DATA[((int)INPUT_DATA[i]) / bin_size] = OUTPUT_DATA[((int)INPUT_DATA[i]) / bin_size] + 1;
    }
}

void CreateHistogram_w_negatives(float *INPUT_DATA, int OUTPUT_DATA[], int bin_size, int data_length, int max_value) {
    for (int i = 0; i < data_length; i++) 
    {
        if(INPUT_DATA[i] < 0)
        {
        OUTPUT_DATA[((int)abs(INPUT_DATA[i])) / bin_size] = OUTPUT_DATA[((int)abs(INPUT_DATA[i])) / bin_size]+ 1;
        }
        else 
        {
        OUTPUT_DATA[(((int)INPUT_DATA[i]) / bin_size)+(max_value/bin_size)] = OUTPUT_DATA[(((int)INPUT_DATA[i]) / bin_size)+(max_value/bin_size)] + 1;
        }
    }
}


#endif // MYFUNCTIONS_H
