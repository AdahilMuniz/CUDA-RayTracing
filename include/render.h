#pragma once
#ifndef __RENDER_H_
#define __RENDER_H_

#include <iostream>
#include <fstream>

#include "Geometry.h"
#include "Sphere.h"
#include "Material.h"
#include "Object.h"
#include "Light.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"

#include "vec4.h"
#include "matrix4.h"
#include "ray.h"
#include "camera.h"
#include "gpu.h"

typedef struct rgb {
    int r,g,b;
} rgb_t;

class Render
{
public:
    
    vec4_t * h_buffer; //Color buffer
    
    //Scene list
    Object ** d_obj_list;
    Light  ** d_light_list;
    int n_obj = 0;
    int n_light = 0;
    
    //Camera
    camera_t * cam;

    //Dimension
    int nx, ny;
    
    Render(int _nx, int _ny);
    ~Render();

    void initWord();
    void renderScene (int nx_block, int ny_block, int nx_thread, int ny_thread, vec4_t * buff );
    void updateCamera (float desloc, float deltaX, float deltaY);
    void getBuffer(vec4_t * color_buffer);
};
#endif