#pragma once
#ifndef __AMBIENT_LIGHT_H_
#define __AMBIENT_LIGHT_H_

#include "Light.h"
#include "vec4.h"

class AmbientLight : public Light
{

public:
    __device__ AmbientLight();
    __device__ AmbientLight(vec4_t intensity);
    __device__ ~AmbientLight();

    __device__ vec4_t Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const;
    __device__ float LightDistance(const vec4_t& point) const;
    __device__ vec4_t GetLightDirection(const vec4_t& point) const;
};

#endif