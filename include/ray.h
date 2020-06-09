#pragma once
#ifndef __RAY_H_
#define __RAY_H_

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

class Geometry;//foward declaration for geometry

typedef struct ray {
    vec4_t origin, direction;
    CUDA_DEV vec4_t point_at(float t) { return (origin + direction*t); }

} ray_t;

typedef struct collision
{
    float distance;
    vec4_t pint; // Intersection Point
    Geometry * geometry = nullptr;

}collision_t;

#endif