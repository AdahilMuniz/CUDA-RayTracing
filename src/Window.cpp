#include "Window.h"

Window::Window(){
    //
}

Window::Window(int _width, int _height, char * _title){
    width = _width;
    height = _height;
    title = _title;
}

Window::~Window(){
    //
}

bool Window::CreateWindow(){

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        cout << "[SDL ERROR]: \n" << SDL_GetError() << endl;
        return false;
    }
    else{
        //Create Window
        window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
        if( window == NULL ){
            cout << "[SDL ERROR]: \n" << SDL_GetError() << endl;
            return false;
        }
        else{
            //Create Screen
            screen = SDL_GetWindowSurface( window );
        }
    }

    return true;
}

void Window::DestroyWindow(){
    //Destroy Window
    SDL_DestroyWindow( window );

    //Quit
    SDL_Quit();
}

void Window::UpdateWindow(vec4_t * color_buffer){
    //
    Uint32 pixel;
    size_t pixel_index;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixel_index = (height-i)*width + (width-j);
            int ir = int(255.99*color_buffer[pixel_index].r);
            int ig = int(255.99*color_buffer[pixel_index].g);
            int ib = int(255.99*color_buffer[pixel_index].b);
            if (ir < 0) ir = 0;
            if (ig < 0) ig = 0;
            if (ib < 0) ib = 0;
            
            //HACK: 0.84*collor
            pixel = SDL_MapRGB( screen->format, (Uint8)(0.84*ir), (Uint8)(0.84*ig), (Uint8)(0.84*ib) );
            setpixel(screen, j, i, pixel);
        }
    }
    SDL_UpdateWindowSurface( window );
}

bool Window::WaitEventWindow(){
    if(SDL_WaitEvent(&event) == 0){
        cout << "ERROR" << endl;
        return false;
    }
    
    if( event.type == SDL_QUIT ){
        quit = true;
        cout << "QUIT" << endl;
        return true;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN){
        //cout << "DOWN" << endl;
        if(event.button.button == SDL_BUTTON_RIGHT){
            mouse_s.right_button = true;
        }
        if(event.button.button == SDL_BUTTON_MIDDLE){
            mouse_s.whell_button = true;
        }
        if(event.button.button == SDL_BUTTON_LEFT){
            mouse_s.left_button = true;
        }
        return true;
    }

    if (event.type == SDL_MOUSEBUTTONUP){
        //cout << "UP" << endl;
        if(event.button.button == SDL_BUTTON_RIGHT){
            mouse_s.right_button = false;
        }
        if(event.button.button == SDL_BUTTON_MIDDLE){
            mouse_s.whell_button = false;
        }
        if(event.button.button == SDL_BUTTON_LEFT){
            mouse_s.left_button = false;
        }
        return true;
    }
    if (event.type == SDL_MOUSEMOTION){
        if(mouse_s.x == 0 || mouse_s.y == 0 ){
            mouse_s.x = event.motion.x;
            mouse_s.y = event.motion.y;
            mouse_s.delta_x = 0;
            mouse_s.delta_y = 0;
        }
        else{
            mouse_s.delta_x = mouse_s.x - event.motion.x;
            mouse_s.delta_y = mouse_s.y - event.motion.y;
            mouse_s.x = event.motion.x;
            mouse_s.y = event.motion.y;
        }
        //cout << "Mouse motion" << "Delta X: " << mouse_s.delta_x<< "Delta Y: " << mouse_s.delta_y <<  endl;
        return true;
    }
    return true;
}

void setpixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        //cout << "I am here" << endl;
        *(Uint32 *)p = pixel;
        break;
    }
}