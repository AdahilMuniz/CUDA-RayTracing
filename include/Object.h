#pragma once
#ifndef __OBJECT_H_
#define __OBJECT_H_

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#define CUDA_HOST    __host__
#define CUDA_DEV     __device__
#else
#define CUDA_HOSTDEV
#define CUDA_HOST
#define CUDA_DEV
#endif

#include "Geometry.h"
#include "Material.h"
#include "vec4.h"
#include <string>

using namespace std;

class Object {

public:
    Geometry *geometry;
    Material *material;
    char * name;
    
    vec4_t position{0,0,0,0};


    CUDA_DEV Object();
    CUDA_DEV Object(Geometry *_geometry);
    CUDA_DEV Object(char * _name, Material * _material, Geometry *_geometry);
    CUDA_DEV ~Object();
};

#endif
