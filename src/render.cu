#include "render.h"

__device__
vec4_t raycast (ray_t ray, Object ** obj_list, Light ** light_list, int n_obj, int n_light, float near, int depth){
    float dist = INFINITY;
    collision_t closest_collision;
    vec4_t accucolor = vec4_t{ 0.8, 0.8, 0.8, 0.0 }; //white room
    //vec4_t accucolor = vec4_t{ 0.0, 0.57, 0.87, 0.0 }; //blue room
    //vec4_t accucolor = vec4_t{ 0.5, 0.5, 0.5, 0.0 }; //Medium room
    Object *closest_obj = nullptr;

    float curr_near = near;
    ray_t curr_ray= ray;

    for (int i = 0; i < n_obj; ++i)
    {
        collision_t col = obj_list[i]->geometry->CollideClosest(curr_ray);//Collision
        float o_dist = col.distance;
        
        if (o_dist < dist && o_dist > curr_near) {
            dist = o_dist;
            closest_collision = col;
            closest_obj = obj_list[i];
        }
    }

    if (closest_obj){
        Material mat = *(closest_obj->material);
        vec4_t normal = closest_obj->geometry->getNormal(closest_collision.pint);
        accucolor = mat.ka;
        
        for (int i = 0; i < n_light; ++i){
    
            vec4_t direction = light_list[i]->GetLightDirection(closest_collision.pint);
            float dot_d_n = dot(direction, normal);
            
            if (dot_d_n >= 0){
                accucolor += (light_list[i]->Illumination(mat, normal, curr_ray.direction * (-1.0), closest_collision.pint)*mat.alfa);
            }
        }
    
        
        if (mat.reflection && depth < 3) {
            const vec4_t reflect = (reflection(ray.direction * (-1.0), normal)).unit();
            accucolor += raycast(ray_t{ closest_collision.pint, reflect}, obj_list, light_list, n_obj, n_light, 0.00001, depth+1) * mat.reflection;
            accucolor = accucolor * 0.5; //HACK
        }

        if (mat.alfa < 1 && depth < 3) {
            vec4_t vec_offset = closest_collision.pint + ray.direction * 0.001; //Avoid bugs
            const vec4_t refract_ray = refract(ray.direction, normal, mat.ior, 1);
            accucolor += raycast(ray_t{ vec_offset , refract_ray }, obj_list, light_list, n_obj, n_light, 0.00001, depth + 1)*(1-mat.alfa);
            accucolor = accucolor * 0.5; //HACK
        }



    }
    return accucolor;
}

__global__
void render (camera_t * cam, Object ** obj_list, Light ** light_list, int dn_obj, int dn_light, vec4_t * buffer) {

    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;

    const vec4_t base = (cam->lookat - cam->position).unit();
    const vec4_t up = cam->up * (cam->dimensions.y / cam->resolution.y);
    const vec4_t left = cross(cam->up, base) * (cam->dimensions.x / cam->resolution.x);

    const int width  = (int)cam->resolution.x;
    const int height = (int)cam->resolution.y;

    const vec4_t origin_screen = base * cam->n + up * (height*(-0.5) + 0.5) + left * (width*(0.5) - 0.5);
    
    if((x >= width) || (y >= height)) return;

    //Scan screen
    vec4_t direction = (origin_screen + up * (y) + left * (-x)).unit();
    buffer[y*width + x] = raycast(ray_t{cam->position, direction}, obj_list, light_list, dn_obj, dn_light, cam->n, 0);
}

__global__
void create_world(Object ** obj_list, Light ** light_list){

    if (threadIdx.x == 0 && blockIdx.x == 0) {
        *(obj_list)     = new Object((char *)"Bronze Sphere", new Material((char *)"Bronze", vec4_t{ 0.2125, 0.1275, 0.054, 0.0 }, vec4_t{ 0.714, 0.4284, 0.18144, 0.0 }, vec4_t{ 0.393548, 0.271906, 0.166721, 0.0 }, 25.6), new Sphere(vec4_t{1.0,0.0,0.0,0.0}, 0.8));
        *(obj_list+1)   = new Object((char *)"Gold Sphere", new Material((char *)"Gold", vec4_t{ 0.24725, 0.1995, 0.0745, 0.0 }, vec4_t{ 0.75164, 0.60648, 0.22648, 0.0 }, vec4_t{ 0.628281, 0.555802, 0.366065, 0.0 }, 51.2), new Sphere(vec4_t{-1.0,0.0,0.0,0.0}, 0.8));
        *(obj_list+2)   = new Object((char *)"Mirror Sphere", new Material((char *)"Mirror", vec4_t{ 1.0, 1.0, 1.0, 0 }, vec4_t{ 1.0, 1.0, 1.0, 0 }, vec4_t{ 1.0, 1.0, 1.0, 0 }, 200.0, 1.0, 0.8), new Sphere(vec4_t{0.0,0.0,-2.0,0.0}, 0.8));
        *(obj_list+3)   = new Object((char *)"Refract Sphere", new Material((char *)"Refract", vec4_t{ 1.0, 1.0, 1.0, 0 }, vec4_t{ 1.0, 1.0, 1.0, 0 }, vec4_t{ 0.2, 0.2, 0.2, 0 }, 2, 0.5, 0.5, 1.1), new Sphere(vec4_t{0.0,0.0,2.0,0.0}, 0.8));
    
        *(light_list  )   = new DirectionalLight(vec4_t{ 1.0, 1.0, 1.0, 0.0 }, vec4_t{ 1.0, 1.0, 1.0, 0.0 });
        *(light_list+1)   = new DirectionalLight(vec4_t{ 1.0, 1.0, 1.0, 0.0 }, vec4_t{ -1.0, 1.0, 1.0, 0.0 });
        *(light_list+2)   = new DirectionalLight(vec4_t{ 1.0, 1.0, 1.0, 0.0 }, vec4_t{ 1.0, -1.0, 1.0, 0.0 });
    }
}

Render::Render(int _nx, int _ny){
    n_obj = 4;
    n_light = 3;

    nx = _nx;
    ny = _ny;
}

Render::~Render(){
    //
}

void Render::initWord(){
    //Allocate Camera
    allocateGPUSharedMem((void **)&cam, sizeof(camera_t));
    *cam = {
        vec4_t{0.0,3.0,10.0,1.0}, //Position
        vec4_t{0.0,0.0,0.0,1.0}, //Lookat
        vec4_t{0.0,1.0,0.0,0.0}, //Up
        vec2_t{(float)nx,(float)ny}, // Resolution
        vec2_t{2.0,2.0}, // Dimension
        2.0 // Near

    };


    //Allocate buffer
    int buffer_space = (int) cam->resolution.x * cam->resolution.y * sizeof(vec4_t);
    allocateGPUSharedMem((void **)&h_buffer, buffer_space);
    //allocateGPUMem((void **)&h_buffer , buffer_space);

    //Allocate scene elements
    allocateGPUMem((void **)&d_obj_list  , sizeof(Object *) * n_obj);
    allocateGPUMem((void **)&d_light_list, sizeof(Light  *) * n_light);

    create_world<<<1,1>>>(d_obj_list, d_light_list);
}

void Render::renderScene (int nx_block, int ny_block, int nx_thread, int ny_thread, vec4_t * buff ){
    //Block and Thread
    dim3 n_blocks (nx_block, ny_block);
    dim3 n_threads(nx_thread, ny_thread);
    //vec4_t a = h_buffer[0];
    
    render<<<n_blocks, n_threads>>>(cam, d_obj_list, d_light_list, n_obj, n_light, h_buffer);
    checkGPUErrors();
    syncGPU();
    //buff = h_buffer;
    cudaMemcpy(buff, h_buffer, (int) nx * ny * sizeof(vec4_t), cudaMemcpyDeviceToHost);
}

void Render::updateCamera (float desloc, float deltaX, float deltaY){
    /*
    *cam = {
        vec4_t{0.0,3.0,10.0,1.0}, //Position
        vec4_t{0.0,0.0,0.0,1.0}, //Lookat
        vec4_t{0.0,1.0,0.0,0.0}, //Up
        vec2_t{(float)nx,(float)ny}, // Resolution
        vec2_t{2.0,2.0}, // Dimension
        2.0 // Near

    };
    */

    //cam->position = cam->position + (cam->position - cam->lookat)*(desloc*0.01f);//Desloc

    //Rotate
    if (deltaX != 0.0 && deltaY != 0.0){
        /* code */
        vec4_t v = cross(cam->lookat - cam->position, cam->up);
        //cam->Transform(rotateArbitrary(-deltaY, v), cam->lookat);
        //cam->Transform(y_rotate(deltaX), cam->lookat);
        cam->Transform(y_rotate(deltaX));
    }


}

void Render::getBuffer(vec4_t * color_buffer){
    //cout << "Test" << endl;
    

    //color_buffer[1] = vec4_t{0,0,0,0};

    /*
    color_buffer = h_buffer;
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            size_t pixel_index = j*nx + i;
            int ir = int(255.99*h_buffer[pixel_index].r);
            int ig = int(255.99*h_buffer[pixel_index].g);
            int ib = int(255.99*h_buffer[pixel_index].b);
            if (ir < 0) ir = 0;
            if (ig < 0) ig = 0;
            if (ib < 0) ib = 0;
            color_buffer[pixel_index].r = ir;
            color_buffer[pixel_index].g = ig;
            color_buffer[pixel_index].b = ib;
            //resultfile << ir << " " << ig << " " << ib << "\n";
        }
    }*/
}