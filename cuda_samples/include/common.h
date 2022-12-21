#include "driver_types.h"
#include "cuda.h"
#include <stdio.h>
#include "cuda_runtime_api.h"

#ifndef FAKE
#define __global__
#define __local__
struct dim3 {
    int x;
    int y;
    int z;
};


static dim3 blockIdx, threadIdx, blockDim, gridDim;



#else


#endif