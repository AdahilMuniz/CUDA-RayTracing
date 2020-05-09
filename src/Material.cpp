#include "Material.h"

Material::Material()
{

}

Material::Material(const string &name, vec4_t ka, vec4_t kd, vec4_t ks, float shininess)
{
    this->name = name;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->shininess = shininess;
    alfa = 1.0f;
    reflection = 0.0f;
}

Material::Material(const string &name, vec4_t ka, vec4_t kd, vec4_t ks, float shininess, float _alpha, float _reflection)
{
    this->name = name;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->shininess = shininess;
    alfa = _alpha;
    reflection = _reflection;
}

Material::Material(const string &name, vec4_t ka, vec4_t kd, vec4_t ks, float shininess, float _ior)
{
    this->name = name;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->shininess = shininess;
    ior = _ior;
}

Material::Material(const string &name, vec4_t ka, vec4_t kd, vec4_t ks, float shininess, float _alpha, float _reflection, float _ior)
{
    this->name = name;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->shininess = shininess;
    alfa = _alpha;
    reflection = _reflection;
    ior = _ior;
}

Material::~Material()
{

}

string Material::getName() const {
    return name;
}