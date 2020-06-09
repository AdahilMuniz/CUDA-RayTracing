#pragma once
#ifndef __GPU_H_
#define __GPU_H_

#include <iostream>

using namespace std;

#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
//void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line);

void allocateGPUSharedMem(void ** ptr, int size);
void allocateGPUMem(void ** ptr, int size);
void freeGPUMem(void* ptr);
void syncGPU();
void checkGPUErrors();

#endif