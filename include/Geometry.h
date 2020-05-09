#pragma once

#ifndef __GEOMETRY_H_
#define __GEOMETRY_H_

//
// Base class for geometry
//

#include <vector>
#include "vec4.h"
#include "ray.h"

using namespace std;

class Geometry
{
public:
    Geometry();
    virtual ~Geometry() = 0;

    virtual collision_t CollideClosest(const ray_t &ray) = 0;//return the closest collision distance of a ray and the geometry
    //virtual RayCollisionList CollideAll(const std::vector<ray_t> &ray) = 0;//return all the colisions of a set of rays
    //virtual RayCollisionList Collide(const ray_t &ray) = 0;//return all collisions of a ray

    virtual vec4_t getNormal(const vec4 &point) = 0; // returns the normal vector at point on a surface

    //virtual void transform(CRAB::Matrix4 m) = 0; // Applies the tranform matrix
};
#endif