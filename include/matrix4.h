#pragma once
#ifndef __MATRIX_4_H_
#define __MATRIX_4_H_

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#define CUDA_HOST    __host__
#define CUDA_DEV     __device__
#else
#define CUDA_HOSTDEV
#define CUDA_HOST
#define CUDA_DEV
#endif

#define PI 3.14

#include "vec4.h"
#include <math.h>

typedef struct matrix4{
    union {
        float m[16];
        float _m[4][4];
    };

    //Access
    CUDA_HOSTDEV inline float  operator[](int i) const { return m[i]; }
    CUDA_HOSTDEV inline float& operator[](int i) { return m[i]; }

    CUDA_HOSTDEV inline float  operator()(int i, int j) const { return _m[i][j]; }
    CUDA_HOSTDEV inline float& operator()(int i, int j) { return _m[i][j]; }

    //Matrix Operations
    CUDA_HOSTDEV inline matrix4 operator * (const matrix4 m0) {
        return 
        matrix4{
            (_m[0][0] * m0._m[0][0]) + (_m[0][1] * m0._m[1][0]) + (_m[0][2] * m0._m[2][0]) + (_m[0][3] * m0._m[3][0]) ,/**/ (_m[0][0] * m0._m[0][1]) + (_m[0][1] * m0._m[1][1]) + (_m[0][2] * m0._m[2][1]) + (_m[0][3] * m0._m[3][1]),/**/ (_m[0][0] * m0._m[0][2]) + (_m[0][1] * m0._m[1][2]) + (_m[0][2] * m0._m[2][2]) + (_m[0][3] * m0._m[3][2]),/**/ (_m[0][0] * m0._m[0][3]) + (_m[0][1] * m0._m[1][3]) + (_m[0][2] * m0._m[2][3]) + (_m[0][3] * m0._m[3][3]),
            (_m[1][0] * m0._m[0][0]) + (_m[1][1] * m0._m[1][0]) + (_m[1][2] * m0._m[2][0]) + (_m[1][3] * m0._m[3][0]) ,/**/ (_m[1][0] * m0._m[0][1]) + (_m[1][1] * m0._m[1][1]) + (_m[1][2] * m0._m[2][1]) + (_m[1][3] * m0._m[3][1]),/**/ (_m[1][0] * m0._m[0][2]) + (_m[1][1] * m0._m[1][2]) + (_m[1][2] * m0._m[2][2]) + (_m[1][3] * m0._m[3][2]),/**/ (_m[1][0] * m0._m[0][3]) + (_m[1][1] * m0._m[1][3]) + (_m[1][2] * m0._m[2][3]) + (_m[1][3] * m0._m[3][3]),
            (_m[2][0] * m0._m[0][0]) + (_m[2][1] * m0._m[1][0]) + (_m[2][2] * m0._m[2][0]) + (_m[2][3] * m0._m[3][0]) ,/**/ (_m[2][0] * m0._m[0][1]) + (_m[2][1] * m0._m[1][1]) + (_m[2][2] * m0._m[2][1]) + (_m[2][3] * m0._m[3][1]),/**/ (_m[2][0] * m0._m[0][2]) + (_m[2][1] * m0._m[1][2]) + (_m[2][2] * m0._m[2][2]) + (_m[2][3] * m0._m[3][2]),/**/ (_m[2][0] * m0._m[0][3]) + (_m[2][1] * m0._m[1][3]) + (_m[2][2] * m0._m[2][3]) + (_m[2][3] * m0._m[3][3]),
            (_m[3][0] * m0._m[0][0]) + (_m[3][1] * m0._m[1][0]) + (_m[3][2] * m0._m[2][0]) + (_m[3][3] * m0._m[3][0]) ,/**/ (_m[3][0] * m0._m[0][1]) + (_m[3][1] * m0._m[1][1]) + (_m[3][2] * m0._m[2][1]) + (_m[3][3] * m0._m[3][1]),/**/ (_m[3][0] * m0._m[0][2]) + (_m[3][1] * m0._m[1][2]) + (_m[3][2] * m0._m[2][2]) + (_m[3][3] * m0._m[3][2]),/**/ (_m[3][0] * m0._m[0][3]) + (_m[3][1] * m0._m[1][3]) + (_m[3][2] * m0._m[2][3]) + (_m[3][3] * m0._m[3][3])
        };
        /*
        */
    }

    CUDA_HOSTDEV vec4_t operator * (const vec4_t &v0) {
        return 
        vec4_t{(v0[0]*_m[0][0] + v0[1]*_m[0][1] + v0[2]*_m[0][2] + v0[3]*_m[0][2]),
               (v0[0]*_m[1][0] + v0[1]*_m[1][1] + v0[3]*_m[1][2] + v0[3]*_m[1][2]),
               (v0[0]*_m[2][0] + v0[1]*_m[2][1] + v0[3]*_m[2][2] + v0[3]*_m[2][2]),
               (v0[0]*_m[3][0] + v0[1]*_m[3][1] + v0[3]*_m[3][2] + v0[3]*_m[3][2])};
        /*
        */
    }

    CUDA_HOSTDEV inline matrix4 transpose(){
        return 
        matrix4{
            _m[0][0], _m[1][0], _m[2][0], _m[3][0],
            _m[0][1], _m[1][1], _m[2][1], _m[3][1],
            _m[0][2], _m[1][0], _m[2][2], _m[3][2],
            _m[0][3], _m[1][3], _m[2][3], _m[3][3],
        };
    }
    /*
    */

} matrix4_t;



CUDA_HOSTDEV inline matrix4_t identity(){
    return matrix4_t{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 };
}

CUDA_HOSTDEV inline matrix4_t translate(const vec4_t &v){
    return matrix4_t{
        1, 0, 0, v.x,
        0, 1, 0, v.y,
        0, 0, 1, v.z,
        0, 0, 0, 1 };
}

CUDA_HOSTDEV inline matrix4_t resize(float x, float y, float z){
    return matrix4_t{
    x,  0,  0,  0,
    0,  y,  0,  0,
    0,  0,  z,  0,
    0,  0,  0,  1 };

}

CUDA_HOSTDEV inline matrix4_t x_rotate(float ang){
    ang = (ang* PI) / 180.0f;

    return matrix4_t{
    1,  0,  0,  0,
    0,  cosf(ang),  -sinf(ang), 0,
    0,  sinf(ang),  cosf(ang),  0,
    0,  0,  0,  1 };
}

CUDA_HOSTDEV inline matrix4_t y_rotate(float ang){
    ang = (ang* PI) / 180.0f;

    return matrix4_t{
    cosf(ang),  0,  sinf(ang),  0,
    0,  1,  0,  0,
    -sinf(ang), 0,  cosf(ang),  0,
    0,  0,  0,  1 };
}

CUDA_HOSTDEV inline matrix4_t z_rotate(float ang){
    ang = (ang* PI) / 180.0f;

    return matrix4_t{
    cosf(ang),  -sinf(ang), 0,  0,
    sinf(ang),  cosf(ang),  0,  0,
    0,  0,  1,  0,
    0,  0,  0,  1 };

}

CUDA_HOSTDEV inline matrix4_t rotateArbitrary(float ang, vec4_t u){
    u.unit();
    ang = (ang* PI) / 180.0f;

    float wq = cosf(ang/2);
    u = u*sinf(ang / 2);

    return matrix4_t{
          (wq*wq) + (u.x*u.x) - (u.y*u.y) - (u.z*u.z) ,   2 * (u.x*u.y) + 2 * (u.z*wq)                ,   2 * (u.x*u.z) - 2 * (u.y*wq)             , 0,
          2 * (u.x*u.y) - 2 * (u.z*wq)                ,   (wq*wq) - (u.x*u.x) + (u.y*u.y) - (u.z*u.z) ,   2 * (u.y*u.z) + 2 * (u.x*wq)             , 0,
          2 * (u.x*u.z) + 2 * (u.y*wq)                ,   2 * (u.y*u.z) - 2 * (u.x*wq)                , (wq*wq) - (u.x*u.x) - (u.y*u.y) + (u.z*u.z), 0,
                                  0                   ,                           0                   ,                          0                 , 1 };
}

#endif