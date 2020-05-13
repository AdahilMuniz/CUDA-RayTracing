#include "Light.h"

__device__ 
Light::Light()
{
}

__device__ 
Light::Light(vec4_t intensity)
{
    this->intensity = intensity;
}

__device__ 
Light::~Light()
{
}
