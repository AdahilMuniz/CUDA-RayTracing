#include "Material.h"

CUDA_DEV
Material::Material()
{

}

CUDA_DEV
Material::Material(char* _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess)
{
    name = _name;
    ka = _ka;
    kd = _kd;
    ks = _ks;
    shininess = _shininess;
    alfa = 1.0f;
    reflection = 0.0f;
}

CUDA_DEV
Material::Material(char* _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess, float _alpha, float _reflection)
{
    name = _name;
    ka = _ka;
    kd = _kd;
    ks = _ks;
    shininess = _shininess;
    alfa = _alpha;
    reflection = _reflection;
}

CUDA_DEV
Material::Material(char* _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess, float _ior)
{
    name = _name;
    ka = _ka;
    kd = _kd;
    ks = _ks;
    shininess = _shininess;
    reflection = 0.0f;
    ior = _ior;
}

CUDA_DEV
Material::Material(char* _name, vec4_t _ka, vec4_t _kd, vec4_t _ks, float _shininess, float _alpha, float _reflection, float _ior)
{
    name = _name;
    ka = _ka;
    kd = _kd;
    ks = _ks;
    shininess = _shininess;
    alfa = _alpha;
    reflection = _reflection;
    ior = _ior;
}

CUDA_DEV
Material::~Material()
{

}

CUDA_DEV
char * Material::getName() const {
    return name;
}