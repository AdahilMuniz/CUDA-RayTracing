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

} camera_t;