#pragma once
#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "Geometry.h"
#include "Material.h"
#include "vec4.h"
#include <string>

using namespace std;

class Object {

public:
    Geometry *geometry;
    Material *material;
    string name;
    
    vec4_t position{0,0,0,0};


    Object();
    Object(Geometry *_geometry);
    Object(const string &_name, Material * _material, Geometry *_geometry);
    ~Object();
};

#endif
