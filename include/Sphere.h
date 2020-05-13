#pragma once
#ifndef __SPHERE_H_
#define __SPHERE_H_

#include "Geometry.h"
#include "ray.h"
#include "vec4.h"

class Sphere : public Geometry{
    
public:
    vec4_t center; // Sphere center
    float radius;  // Sphere radius
    
    __device__ Sphere();
    __device__ Sphere(const vec4_t &C, float R);
    __device__ ~Sphere();

    //ray_t CollisionList CollideAll(const std::vector<ray_t> &ray);
    __device__ collision_t CollideClosest(const ray_t &ray);
    //ray_t CollisionList Collide(const ray_t &ray);

    __device__ vec4_t getNormal(const vec4_t &point);

    //void transform(CRAB::Matrix4 m);
};

#endif