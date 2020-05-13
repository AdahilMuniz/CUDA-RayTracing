#pragma once
#ifndef __RAY_H_
#define __RAY_H_

#include "vec4.h"

class Geometry;//foward declaration for geometry

typedef struct ray {
    vec4_t origin, direction;
    __device__ vec4_t point_at(float t) { return (origin + direction*t); }

} ray_t;

typedef struct collision
{
    float distance;
    vec4_t pint; // Intersection Point
    Geometry * geometry = nullptr;

}collision_t;

#endif