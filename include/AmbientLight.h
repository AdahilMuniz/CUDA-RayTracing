#pragma once
#ifndef __AMBIENT_LIGHT_H_
#define __AMBIENT_LIGHT_H_

#include "Light.h"
#include "vec4.h"

class AmbientLight : public Light
{

public:
    AmbientLight();
    AmbientLight(vec4_t intensity);
    ~AmbientLight();

    vec4_t Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const;
    float LightDistance(const vec4_t& point) const;
    vec4_t GetLightDirection(const vec4_t& point) const;
};

#endif