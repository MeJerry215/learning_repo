#include <cuda.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "common.h"
void SumArrayOnHost(float *a, float *b, float *c, const int n)
{
    for (int i = 0; i < n; i++)
    {
        c[i] = a[i] + b[i];
    }
}

void init_data(float *p, int size)
{
    srand(42);
    for (int i = 0; i < size; i++)
    {
        p[i] = (float)(rand() & 0xff) / 10.0f;
    }
}

bool check_result(float* c, float* e, const int n) {
    for(int i = 0; i < n; i++) {
        if (fabs(c[i] - e[i]) > 1e-3) return false;
    }
    return true;
}

__global__ void SumArrayGpu(float *a, float *b, float *c, const int n)
{
    int tx = threadIdx.x, bx = blockIdx.x;
    int tid = blockDim.x * bx + tx;
    if (tid >= n)  return;
    c[tid] = a[tid] + b[tid];
}

int main(int argc, char** argv) {
    const int N = 1024;
    size_t nBytes = N * sizeof(float);
    float *h_a, *h_b, *h_c, *e_c;
    float *d_a, *d_b, *d_c;
    h_a = (float *)malloc(nBytes);
    h_b = (float *)malloc(nBytes);
    h_c = (float *)malloc(nBytes);
    e_c = (float *)malloc(nBytes);
    init_data(h_a, N);
    init_data(h_b, N);
    cudaMalloc((void**)&d_a, nBytes);
    cudaMalloc((void**)&d_b, nBytes);
    cudaMalloc((void**)&d_c, nBytes);
    cudaMemcpy(d_a, h_a, nBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, nBytes, cudaMemcpyHostToDevice);
    SumArrayOnHost(h_a, h_b, e_c, N);
    SumArrayGpu<<<32, 32>>>(d_a, d_b, d_c, N);
    cudaMemcpy(h_c, d_c, nBytes, cudaMemcpyDeviceToHost);
    if (check_result(h_c, e_c, N)) {
        printf("result equal\n");
    } else {
        printf("result not same\n");
    }
    free(h_a);
    free(h_b);
    free(h_c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    cudaFree(e_c);
}