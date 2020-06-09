#pragma once
#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "matrix4.h"
#include "vec4.h"

typedef struct vec2 {
        float x, y;
} vec2_t;

typedef struct camera {

        vec4_t position;
        vec4_t lookat;
        vec4_t up;

        vec2_t resolution;
        vec2_t dimensions;

        float n;//near plane

        void Transform(matrix4_t m){
            //lookat = m * lookat;
            //up = (m * up).unit();
            position = m * position;
        }

        void Transform(matrix4_t m, vec4_t reference){
            Transform(translate(vec4_t{ -reference.x, -reference.y, -reference.z, 1.0 }));

            Transform(m);

            Transform(translate(vec4_t{ reference.x, reference.y, reference.z, 1.0 }));
        }

} camera_t;

#endif