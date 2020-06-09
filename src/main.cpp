#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include "matrix4.h"

#include "gpu.h"
#include "render.h"
#include "Window.h"

using namespace std;

int main(int argc, char const *argv[])
{
    //Result File
    ofstream resultfile ("result.ppm");
    //Dimensions
    int nx = 512;
    int ny = 512;

    //Thread Size
    int tx = 8;
    int ty = 8;

    //Color Buffer
    vec4_t * color_buffer;


    //Allocate CollorBuffer
    color_buffer = (vec4_t *) malloc(nx*ny*sizeof(vec4_t));
    
    //Render Object
    Render * rend = new Render(nx, ny);
    
    //Window Object
    Window * wind = new Window(nx, ny, (char *)"CUDA RayTracing");

    rend->initWord(); //Set camera, collor buffer, object and lights
    wind->CreateWindow(); //Create the qindow and screen

    //Render
    while(!(wind->quit)){
        //clock_t begin = clock();
        rend->renderScene(nx+1/tx, ny+1/ty, tx, ty, color_buffer); //Render scene and get the collor buffer
        //clock_t end = clock();
        //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        //cout << "It spent: " << time_spent <<"s" << " to render." << endl;
        wind->UpdateWindow((vec4_t*)color_buffer);
        wind->WaitEventWindow();
        //if (wind->mouse_s.right_button){
        //    rend->updateCamera((float)wind->mouse_s.delta_y, 0.0,0.0);
        //}
        if (wind->mouse_s.left_button){
            rend->updateCamera(0.0, (float)wind->mouse_s.delta_x, (float)wind->mouse_s.delta_y);
        }
    }

    wind->DestroyWindow();

    return 0;
}