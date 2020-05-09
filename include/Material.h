#pragma once
#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include "vec4.h"
#include <string>

using namespace std;

class Material
{
public:
    string name;
    vec4_t ka; // Ambient Coefficient
    vec4_t kd; // Diffuse Coefficient
    vec4_t ks; // Spectral Coefficient
    float shininess;    // Shininess
    float alfa = 1;     // Transparency [0.0; 1.0]
    float reflection;   // [0.0; 1.0]
    float ior = 1;      //Refractive Index

    Material();
    Material(const string &name, vec4_t ka, vec4_t kd, vec4_t ks, float shininess);
    Material(const string &name, vec4_t ka, vec4_t kd, vec4_t ks, float shininess, float _alpha, float _reflection);
    Material(const string &name, vec4_t ka, vec4_t kd, vec4_t ks, float shininess, float _ior);
    Material(const string &name, vec4_t ka, vec4_t kd, vec4_t ks, float shininess, float _alpha, float _reflection, float _ior);

    ~Material();

    string getName() const;
};

#endif