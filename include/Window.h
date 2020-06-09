#pragma once
#ifndef __WINDOW_H_
#define __WINDOW_H_

#include <iostream>
#include <SDL2/SDL.h>

#include "vec4.h"

using namespace std;

typedef struct wrgb{
    union {
        struct { float r, g, b;};
        float v[3];
    };
} wrgb_t;

typedef struct mouse_state{
    int delta_x, delta_y;
    int x =0 , y = 0;
    bool right_button = false, left_button = false, whell_button = false;

} mouse_state_t;

void setpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

class Window
{
public:

    SDL_Window* window = NULL; //SDL Window
    SDL_Surface* screen = NULL; //SDL Screen
    SDL_Renderer *renderer = NULL;
    SDL_Texture * texture = NULL;

    //Configure Attributes
    int width ;
    int height;
    char * title;

    //Events
    SDL_Event event;
    bool quit = false;
    mouse_state_t mouse_s;


    Window();
    Window(int _width, int _height, char * _title);
    ~Window();

    bool CreateWindow();
    void DestroyWindow();
    void UpdateWindow(vec4_t * color_buffer);
    bool WaitEventWindow();
    
};
#endif