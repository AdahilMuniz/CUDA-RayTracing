#include "Object.h"


Object::Object()
{
}

Object::Object(Geometry *_geometry)
{
    geometry = _geometry;
    name = "untitled"; // default name
    material = new Material("Neutral", vec4_t{ 0.3f, 0.3f, 0.3f, 0.0f }, vec4_t{ 1.0f, 1.0f, 1.0f, 0.0f }, vec4_t{ 1.0f, 1.0f, 1.0f, 0.0f }, 10.0f); // default color
}

Object::Object(const string &_name, Material * _material, Geometry *_geometry) : name(_name), material(_material), geometry(_geometry)
{
}

Object::~Object()
{
}