#pragma once
#ifndef __LIGHT_H_
#define __LIGHT_H_

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
#include "Material.h"

//inline vec4_t reflection(const vec4_t& vi, const vec4_t& plan) { return (plan * dot(plan, vi))*(2.0) - vi; }

class Light
{
public:
    vec4_t intensity;

    CUDA_DEV Light();
    CUDA_DEV Light(vec4_t intensity);
    CUDA_DEV virtual ~Light() = 0;

    //Return the collor after the interation with the light and material
    CUDA_DEV virtual vec4_t Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const = 0;
    CUDA_DEV virtual float LightDistance(const vec4_t &point) const = 0;
    CUDA_DEV virtual vec4_t GetLightDirection(const vec4_t& point) const = 0;

};

#endif