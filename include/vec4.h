#pragma once
#ifndef __VEC_4_H_
#define __VEC_4_H_

#include <math.h>

typedef struct vec4{
    union {
        struct { float x, y, z, w;};
        struct { float r, g, b, l;};
        float v[4];
    };

    //Access
    inline float  operator[](int i) const { return v[i]; }
    inline float& operator[](int i) { return v[i]; };

    //Vector Operations
    inline void operator += (const vec4& _v) { x += _v.x; y += _v.y; z += _v.z; }
    inline void operator -= (const vec4& _v) { x -= _v.x; y -= _v.y; z -= _v.z; }
    inline void operator *= (const vec4& _v) { x *= _v.x; y *= _v.y; z *= _v.z; }

    inline vec4 operator + (const vec4& _v) const { vec4 vr{0.0,0.0,0.0,1.0}; vr.x = x + _v.x; vr.y = y + _v.y; vr.z = z + _v.z; return vr;}
    inline vec4 operator - (const vec4& _v) const { vec4 vr{0.0,0.0,0.0,1.0}; vr.x = x - _v.x; vr.y = y - _v.y; vr.z = z - _v.z; return vr;}
    inline vec4 operator * (const vec4& _v) const { vec4 vr{0.0,0.0,0.0,1.0}; vr.x = x * _v.x; vr.y = y * _v.y; vr.z = z * _v.z; return vr;}

    inline vec4 operator -  () const { return vec4{-x,-y,-z,1.0}; }


    //Scalar Operations
    inline void operator += (const float& a) { x += a; y += a; z += a; }
    inline void operator -= (const float& a) { x -= a; y -= a; z -= a; }
    inline void operator *= (const float& a) { x *= a; y *= a; z *= a; }
    inline void operator /= (const float& a) { x /= a; y /= a; z /= a; }

    inline vec4 operator + (const float& a) const { vec4 vr{0.0,0.0,0.0,1.0}; vr.x = x + a; vr.y = y + a; vr.z = z + a; return vr;}
    inline vec4 operator - (const float& a) const { vec4 vr{0.0,0.0,0.0,1.0}; vr.x = x - a; vr.y = y - a; vr.z = z - a; return vr;}
    inline vec4 operator * (const float& a) const { vec4 vr{0.0,0.0,0.0,1.0}; vr.x = x * a; vr.y = y * a; vr.z = z * a; return vr;}
    inline vec4 operator / (const float& a) const { vec4 vr{0.0,0.0,0.0,1.0}; vr.x = x / a; vr.y = y / a; vr.z = z / a; return vr;}

    //
    inline float length()    const { return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]); }
    inline float length_sq() const { return v[0]*v[0] + v[1]*v[1] + v[2]*v[2]; }
    inline vec4  unit()      const { return vec4{ x/length(), y/length(), z/length(), w }; }

} vec4_t;


inline float dot  (const vec4& va, const vec4& vb) { return va.x*vb.x + va.y*vb.y + va.z*vb.z; }
inline vec4  cross(const vec4& va, const vec4& vb) { return { va.y*vb.z - va.z*vb.y, va.z*vb.x - va.x*vb.z, va.x*vb.y - va.y*vb.x, 0.0f }; }

inline vec4_t reflection(const vec4_t& vi, const vec4_t& plan) { return (plan * dot(plan, vi))*(2.0) - vi; }

inline vec4_t refract(const vec4_t & incident, const vec4_t & normal, const float & ior_t, const float & ior_i) {
    //T = eta*I + (eta*C1 - C2)*N
    //eta = etai/etat -> eta
    //C1 = cos(theta) = N.I -> n_dot_i
    //C2 = sqrt(1-eta²*(1-cos²(theta))) -> k
    
    vec4_t Nrefr;
    float n_dot_i = dot(normal, incident); //N.I = cos(theta)
    float etai, etat, eta, k;

    if (n_dot_i > 1){
        n_dot_i = 1;
    }
    else if(n_dot_i < -1){
        n_dot_i = -1;
    }

    if (n_dot_i < 0){//Entering the surface (cos(theta)<0)
        n_dot_i = -n_dot_i;
        Nrefr = normal;
        etai = ior_i;
        etat = ior_t;
    }
    else{//Leaving the surface (Reverse Normal)
        Nrefr = normal*(-1);
        etai = ior_t;
        etat = ior_i;
    }
    eta = etai / etat; //eta
    k = 1 - (eta * eta * (1 - n_dot_i * n_dot_i));

    if (k < 0) {// Total internal reflection (There is no refraction in this case)
        return vec4_t{0.0f, 0.0f, 0.0f, 0.0f};
    }

    return ((incident*eta) + Nrefr*(eta*n_dot_i - sqrtf(k))).unit();

}

#endif