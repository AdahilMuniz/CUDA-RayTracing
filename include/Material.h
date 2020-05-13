#pragma once
#ifndef __MATERIAL_H_
#define __MATERIAL_H_

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

    __device__ Material();
    __device__ Material(char * _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess);
    __device__ Material(char * _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess, float _alpha, float _reflection);
    __device__ Material(char * _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess, float _ior);
    __device__ Material(char * _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess, float _alpha, float _reflection, float _ior);

    __device__ ~Material();

    __device__ char * getName() const;
};

#endif