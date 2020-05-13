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
    char * name;
    
    vec4_t position{0,0,0,0};


    __device__ Object();
    __device__ Object(Geometry *_geometry);
    __device__ Object(char * _name, Material * _material, Geometry *_geometry);
    __device__ ~Object();
};

#endif
