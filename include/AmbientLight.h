#pragma once
#ifndef __AMBIENT_LIGHT_H_
#define __AMBIENT_LIGHT_H_

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#define CUDA_HOST    __host__
#define CUDA_DEV     __device__
#else
#define CUDA_HOSTDEV
#define CUDA_HOST
#define CUDA_DEV
#endif

#include "Light.h"
#include "vec4.h"

class AmbientLight : public Light
{

public:
    CUDA_DEV AmbientLight();
    CUDA_DEV AmbientLight(vec4_t intensity);
    CUDA_DEV ~AmbientLight();

    CUDA_DEV vec4_t Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const;
    CUDA_DEV float LightDistance(const vec4_t& point) const;
    CUDA_DEV vec4_t GetLightDirection(const vec4_t& point) const;
};

#endif