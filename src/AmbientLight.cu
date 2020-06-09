#include "AmbientLight.h"

CUDA_DEV 
AmbientLight::AmbientLight()
{

}

CUDA_DEV 
AmbientLight::AmbientLight(vec4_t intensity) : Light(intensity)
{
}

CUDA_DEV 
AmbientLight::~AmbientLight(){

}

CUDA_DEV 
vec4_t AmbientLight::Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const {
    vec4_t color;
    color = this->intensity * mat.ka;

    return color;
}

CUDA_DEV 
float AmbientLight::LightDistance(const vec4_t& point) const {
    return 0;
}

CUDA_DEV 
vec4_t AmbientLight::GetLightDirection(const vec4_t& point) const {
    return vec4_t{0.0, 0.0, 0.0, 0.0};
}