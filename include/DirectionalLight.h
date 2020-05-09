#pragma once
#ifndef __DIRECTIONAL_LIGHT_H_
#define __DIRECTIONAL_LIGHT_H_

#include "Light.h"
#include "vec4.h"
class DirectionalLight :
    public Light
{
public:
    vec4_t direction;
    DirectionalLight();

    DirectionalLight(vec4_t _intensity, vec4_t _direction);

    ~DirectionalLight();

    vec4_t Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const;
    float LightDistance(const vec4_t& point) const;
    vec4_t GetLightDirection(const vec4_t& point) const;
};

#endif