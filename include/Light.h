#pragma once
#ifndef __LIGHT_H_
#define __LIGHT_H_

#include "vec4.h"
#include "Material.h"

//inline vec4_t reflection(const vec4_t& vi, const vec4_t& plan) { return (plan * dot(plan, vi))*(2.0) - vi; }

class Light
{
public:
    vec4_t intensity;

    __device__ Light();
    __device__ Light(vec4_t intensity);
    __device__ virtual ~Light() = 0;

    //Return the collor after the interation with the light and material
    __device__ virtual vec4_t Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const = 0;
    __device__ virtual float LightDistance(const vec4_t &point) const = 0;
    __device__ virtual vec4_t GetLightDirection(const vec4_t& point) const = 0;

};

#endif