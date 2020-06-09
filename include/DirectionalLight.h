#pragma once
#ifndef __DIRECTIONAL_LIGHT_H_
#define __DIRECTIONAL_LIGHT_H_

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
class DirectionalLight :
    public Light
{
public:
    vec4_t direction;
    CUDA_DEV DirectionalLight();

    CUDA_DEV DirectionalLight(vec4_t _intensity, vec4_t _direction);

    CUDA_DEV ~DirectionalLight();

    CUDA_DEV vec4_t Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const;
    CUDA_DEV float LightDistance(const vec4_t& point) const;
    CUDA_DEV vec4_t GetLightDirection(const vec4_t& point) const;
};

#endif