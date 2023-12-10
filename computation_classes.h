// computation_classes.h

#ifndef COMPUTATION_CLASSES_H
#define COMPUTATION_CLASSES_H

#include <cstddef> // for size_t
#include <tbb/blocked_range.h>

class Max {
public:
    float *my_a;
    float my_max;
    int my_index;

    void operator() (const tbb::blocked_range<size_t> &r) {
        float *a = my_a;
        float max = my_max;
        int index = my_index; 

        // Get the maximum for a portion of the vector:
        for (size_t i = r.begin(); i != r.end(); i++) { 
            if (a[i] >= max) {
                max = a[i];
                index = i;
            } 
        }   
        my_max = max;
        my_index = index;
    }

    Max (Max &x, tbb::split): my_a (x.my_a), my_max(x.my_a[0]), my_index(0) {}
    void join (const Max &y) { 
        if (y.my_max > my_max) 
            my_max = y.my_max; 
        my_index = y.my_index;
    }

    Max (float *a): my_a(a), my_max(my_a[0]), my_index(0) {} 
    Max () {}
};

class Min {
public:
    float *my_a;
    float my_min;
    int my_index;

    void operator() (const tbb::blocked_range<size_t> &r) {
        float *a = my_a;
        float min = my_min;
        int index = my_index;

        // Get the minimum for a portion of the vector:
        for (size_t i = r.begin(); i != r.end(); i++) { 
            if (a[i] <= min) {   
                min = a[i];
                index = i;
            }
        }
        my_min = min;
        my_index = index;
    }

    Min (Min &x, tbb::split): my_a (x.my_a), my_min(x.my_a[0]), my_index(0) {}
    void join (const Min &y) { 
        if (y.my_min < my_min) 
            my_min = y.my_min; 
        my_index = y.my_index; 
    }

    Min (float *a): my_a(a), my_min(my_a[0]), my_index(0) {} 
    Min () {}
};

class SumFun {
    int **my_a;
    int my_bin_size;
    int my_max_value;

public:
    int *my_sum;

    void operator() (const tbb::blocked_range<int> &r)  {
        int **a = my_a;
        int *sum = my_sum;
        int bin_size = my_bin_size;
        int max_value = my_max_value;

        for (int i=r.begin(); i != r.end(); ++i) {
            for(int j = 0; j < (max_value/bin_size); ++j) {
                sum[j] += a[i][j];
            }
        }    
        my_sum = sum;
    }

    SumFun (int **a, int bin_size, int max_value): my_a(a), my_sum(new int[(max_value/bin_size)]()), my_bin_size(bin_size), my_max_value(max_value) {}

    SumFun (SumFun &x, tbb::split): my_a(x.my_a), my_sum(new int[x.my_max_value/x.my_bin_size]()), my_bin_size(x.my_bin_size), my_max_value(x.my_max_value) {}

    void join (const SumFun &y) {
        for(int j = 0; j < (my_max_value/my_bin_size); ++j) {
            my_sum[j] += y.my_sum[j]; 
        } 
    }
};

#endif // COMPUTATION_CLASSES_H
