#include "Object.h"

__device__
Object::Object()
{
}

__device__
Object::Object(Geometry *_geometry)
{
    geometry = _geometry;
    name = (char *)"untitled"; // default name
    material = new Material((char *)"Neutral", vec4_t{ 0.3f, 0.3f, 0.3f, 0.0f }, vec4_t{ 1.0f, 1.0f, 1.0f, 0.0f }, vec4_t{ 1.0f, 1.0f, 1.0f, 0.0f }, 10.0f); // default color
}

__device__
Object::Object(char * _name, Material * _material, Geometry *_geometry) : name(_name), material(_material), geometry(_geometry)
{
}

__device__
Object::~Object()
{
}