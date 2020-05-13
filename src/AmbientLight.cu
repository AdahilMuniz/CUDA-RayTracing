#include "AmbientLight.h"

__device__ 
AmbientLight::AmbientLight()
{

}

__device__ 
AmbientLight::AmbientLight(vec4_t intensity) : Light(intensity)
{
}

__device__ 
AmbientLight::~AmbientLight(){

}

__device__ 
vec4_t AmbientLight::Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const {
    vec4_t color;
    color = this->intensity * mat.ka;

    return color;
}

__device__ 
float AmbientLight::LightDistance(const vec4_t& point) const {
    return 0;
}

__device__ 
vec4_t AmbientLight::GetLightDirection(const vec4_t& point) const {
    return vec4_t{0.0, 0.0, 0.0, 0.0};
}