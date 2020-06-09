#include "Light.h"

CUDA_DEV 
Light::Light()
{
}

CUDA_DEV 
Light::Light(vec4_t intensity)
{
    this->intensity = intensity;
}

CUDA_DEV 
Light::~Light()
{
}
