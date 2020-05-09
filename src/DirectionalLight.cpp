#include "DirectionalLight.h"



DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(vec4_t _intensity, vec4_t _direction): 
    Light(_intensity),
    direction(_direction.unit()){
}

DirectionalLight::~DirectionalLight()
{
}

vec4_t DirectionalLight::Illumination(const Material &mat, const vec4_t &normal, const vec4_t &view, const vec4_t &P) const {

    vec4_t L = direction * (-1.0);
    vec4_t Id;// diffuse reflection
    vec4_t Is;// specular reflection

    float dot_L_n = dot(L, normal);
    
    if (dot_L_n > 0.0) {
        Id = (intensity * mat.kd) * dot_L_n; //diffuse reflection
    }

    //vec4_t r = reflection(L, normal);
    float dot_r_v = dot(reflection(L, normal), view);
    if (dot_r_v > 0.0) {
        Is = (intensity * mat.ks) * powf(dot_r_v, mat.shininess);//specular reflection
    }
    return Id + Is;
}


float DirectionalLight::LightDistance(const vec4_t& point) const {
    return INFINITY;
}

vec4_t DirectionalLight::GetLightDirection(const vec4_t& point) const {
    return direction * (-1.0);
}