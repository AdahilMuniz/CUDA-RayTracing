#include "gpu.h"

//Got directly from "Accelerated Ray Tracing in One Weekend in CUDA" tutorial
void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line) {
    if (result) {
        cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
            file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();
        exit(99);
    }
}

void allocateGPUSharedMem(void ** ptr, int size){
    checkCudaErrors(cudaMallocManaged((void **)ptr, size));
}

void allocateGPUMem(void ** ptr, int size){
    checkCudaErrors(cudaMalloc((void **)ptr  , size));
}

void freeGPUMem(void* ptr){
    checkCudaErrors(cudaFree(ptr));
}

void syncGPU(){
    checkCudaErrors(cudaDeviceSynchronize());
}

void checkGPUErrors(){
    checkCudaErrors(cudaGetLastError());
}
