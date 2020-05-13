#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>

#include "Geometry.h"
#include "Sphere.h"
#include "Material.h"
#include "Object.h"
#include "Light.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"

#include "vec4.h"
#include "ray.h"
#include "camera.h"

using namespace std;

//Got directly from "Accelerated Ray Tracing in One Weekend in CUDA" tutorial
#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line) {
    if (result) {
        cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
            file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();
        exit(99);
    }
}

__device__ vec4_t raycast (ray_t ray, Object ** obj_list, Light ** light_list, int n_obj, int n_light, float near, int depth);
__global__ void render (camera_t * cam, Object ** obj_list, Light ** light_list, int n_obj, int n_light, vec4_t * buffer);
__global__ void create_world(Object ** obj_list, Light ** light_list);


int main(int argc, char const *argv[])
{
    //REsult File
    ofstream resultfile ("result.ppm");
    //Dimensions
    int nx = 512;
    int ny = 512;
    //int ns = 10;
    //Thread Size
    int tx = 8;
    int ty = 8;
    //Block and Thread
    dim3 n_blocks (nx+1/tx, ny+1/ty);
    dim3 n_threads(tx, ty);

    vec4_t * buffer; //Color buffer
    
    //Scene list
    Object ** d_obj_list;
    Light  ** d_light_list;

    //Camera
    camera_t * cam;

    //Allocate Camera
    checkCudaErrors(cudaMallocManaged((void **)&cam, sizeof(camera_t)));
    *cam = {
        vec4_t{0.0,3.0,10.0,1.0}, //Position
        vec4_t{0.0,0.0,0.0,1.0}, //Lookat
        vec4_t{0.0,1.0,0.0,0.0}, //Up
        vec2_t{(float)nx,(float)ny}, // Resolution
        vec2_t{2.0,2.0}, // Dimension
        2.0 // Near

    };


    //Allocate buffer
    float bufer_space = cam->resolution.x * cam->resolution.y * sizeof(vec4_t);
    checkCudaErrors(cudaMallocManaged((void **)&buffer, bufer_space));

    //Allocate scene elements
    checkCudaErrors(cudaMalloc((void **)&d_obj_list  , sizeof(Object *) * 4));
    checkCudaErrors(cudaMalloc((void **)&d_light_list, sizeof(Light  *) * 3));
    create_world<<<1,1>>>(d_obj_list, d_light_list);

    //Render
    clock_t begin = clock();
    render<<<n_blocks, n_threads>>>(cam, d_obj_list, d_light_list, 4, 3, buffer);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "It spent: " << time_spent <<"s" << " to render." << endl;

    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    //Generate PPM Image
    resultfile << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            size_t pixel_index = j*nx + i;
            int ir = int(255.99*buffer[pixel_index].r);
            int ig = int(255.99*buffer[pixel_index].g);
            int ib = int(255.99*buffer[pixel_index].b);
            if (ir < 0) ir = 0;
            if (ig < 0) ig = 0;
            if (ib < 0) ib = 0;
            resultfile << ir << " " << ig << " " << ib << "\n";
        }
    }


    return 0;
}

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

__device__
vec4_t test_func(){
    return vec4_t{1.0,1.0,1.0,1.0};
}

__global__
void render (camera_t * cam, Object ** obj_list, Light ** light_list, int n_obj, int n_light, vec4_t * buffer) {

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
    buffer[y*width + x] = raycast(ray_t{cam->position, direction}, obj_list, light_list, n_obj, n_light, cam->n, 0);


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
