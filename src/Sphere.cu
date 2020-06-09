#include "Sphere.h"

CUDA_DEV
Sphere::Sphere()
{

}

CUDA_DEV
Sphere::Sphere(const vec4_t &C, float R){
    center = C;
    radius = R;
}

CUDA_DEV
Sphere::~Sphere()
{

}

//TODO: Get the Intercept Point
CUDA_DEV
collision_t Sphere::CollideClosest(const ray_t &ray)
{
    collision_t col;
    col.geometry = this;

    const vec4_t &W = ray.origin - center;     // The difference of P0 - C

    // Coefficients of the equation
    //const float A = 1.0f;//considering that the direction is already normalized
    const float B = dot(W, ray.direction);
    const float C = dot(W, W) - (radius * radius);
    // Discriminant
    const float Delta = (B*B - C);

    if (Delta < 0.0f) { col.distance = INFINITY; return col; }

    //Distance
    col.distance = -(B + sqrtf(Delta));

    //Intersection Point
    col.pint = ray.origin + (ray.direction * col.distance); // Surface 

    return col;
}

CUDA_DEV
vec4_t Sphere::getNormal(const vec4_t &point)
{
    vec4_t n = point - center;
    n.unit();
    return n;
}