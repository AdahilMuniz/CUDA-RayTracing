#pragma once
#ifndef __SPHERE_H_
#define __SPHERE_H_

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
#include "ray.h"
#include "vec4.h"

class Sphere : public Geometry{
    
public:
    vec4_t center; // Sphere center
    float radius;  // Sphere radius
    
    CUDA_DEV Sphere();
    CUDA_DEV Sphere(const vec4_t &C, float R);
    CUDA_DEV ~Sphere();

    //ray_t CollisionList CollideAll(const std::vector<ray_t> &ray);
    CUDA_DEV collision_t CollideClosest(const ray_t &ray);
    //ray_t CollisionList Collide(const ray_t &ray);

    CUDA_DEV vec4_t getNormal(const vec4_t &point);

    //void transform(CRAB::Matrix4 m);
};

#endif