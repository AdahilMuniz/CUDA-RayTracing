#pragma once

#ifndef __GEOMETRY_H_
#define __GEOMETRY_H_

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#define CUDA_HOST    __host__
#define CUDA_DEV     __device__
#else
#define CUDA_HOSTDEV
#define CUDA_HOST
#define CUDA_DEV
#endif

//
// Base class for geometry
//

#include <vector>
#include "vec4.h"
#include "ray.h"

using namespace std;

class Geometry
{
public:
    CUDA_DEV Geometry();
    CUDA_DEV virtual ~Geometry() = 0;

    CUDA_DEV virtual collision_t CollideClosest(const ray_t &ray) = 0;//return the closest collision distance of a ray and the geometry
    //virtual RayCollisionList CollideAll(const std::vector<ray_t> &ray) = 0;//return all the colisions of a set of rays
    //virtual RayCollisionList Collide(const ray_t &ray) = 0;//return all collisions of a ray

    CUDA_DEV virtual vec4_t getNormal(const vec4 &point) = 0; // returns the normal vector at point on a surface

    //virtual void transform(CRAB::Matrix4 m) = 0; // Applies the tranform matrix
};
#endif