#pragma once
#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#define CUDA_HOST    __host__
#define CUDA_DEV     __device__
#else
#define CUDA_HOSTDEV
#define CUDA_HOST
#define CUDA_DEV
#endif

#include "vec4.h"
#include <string>

using namespace std;

class Material
{
public:
    char * name;
    vec4_t ka; // Ambient Coefficient
    vec4_t kd; // Diffuse Coefficient
    vec4_t ks; // Spectral Coefficient
    float shininess;    // Shininess
    float alfa = 1;     // Transparency [0.0; 1.0]
    float reflection;   // [0.0; 1.0]
    float ior = 1;      //Refractive Index

    CUDA_DEV Material();
    CUDA_DEV Material(char * _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess);
    CUDA_DEV Material(char * _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess, float _alpha, float _reflection);
    CUDA_DEV Material(char * _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess, float _ior);
    CUDA_DEV Material(char * _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess, float _alpha, float _reflection, float _ior);

    CUDA_DEV ~Material();

    CUDA_DEV char * getName() const;
};

#endif