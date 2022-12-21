#include "common.h"

__global__ void HelloWorldFromGPU() {
    printf("hello world from GPU!\n");
}

int main(int argc, char** argv) {
    printf("hello world from CPU\n");
    // cudaSetDevice(0);
    HelloWorldFromGPU<<<1, 10>>>();
    cudaDeviceSynchronize();
    // cudaDeviceReset();
    // cudaMemcpyHostToDevice
}