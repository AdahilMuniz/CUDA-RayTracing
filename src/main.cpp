#include <iostream>
#include <vector>

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

vec4_t raycast (ray_t ray, const vector<Object *> &obj_list, const vector<Light *> &light_list, float near, int depth);
void render (const camera_t &cam, const vector<Object *> &obj_list, const vector<Light *> &light_list , vec4_t * buffer);

int main(int argc, char const *argv[])
{
    int nx = 512;
    int ny = 512;
    int ns = 10;

    camera_t cam{
        vec4_t{0.0,3.0,10.0,1.0}, //Position
        vec4_t{0.0,0.0,0.0,1.0}, //Lookat
        vec4_t{0.0,1.0,0.0,0.0}, //Up
        vec2_t{512.0,512.0}, // Resolution
        vec2_t{2.0,2.0}, // Dimension
        2.0 // Near

    };

    int bufer_space = cam.resolution.x * cam.resolution.y * sizeof(vec4_t);

    vec4_t * buffer = new(nothrow) vec4_t[bufer_space];

    vector<Object *> obj_list;
    vector<Light *> light_list;

    obj_list.push_back(new Object("Bronze Sphere", new Material("Bronze", vec4_t{ 0.2125, 0.1275, 0.054, 0.0 }, vec4_t{ 0.714, 0.4284, 0.18144, 0.0 }, vec4_t{ 0.393548, 0.271906, 0.166721, 0.0 }, 25.6), new Sphere(vec4_t{1.0,0.0,0.0,0.0}, 0.8)));
    obj_list.push_back(new Object("Gold Sphere", new Material("Gold", vec4_t{ 0.24725, 0.1995, 0.0745, 0.0 }, vec4_t{ 0.75164, 0.60648, 0.22648, 0.0 }, vec4_t{ 0.628281, 0.555802, 0.366065, 0.0 }, 51.2), new Sphere(vec4_t{-1.0,0.0,0.0,0.0}, 0.8)));
    obj_list.push_back(new Object("Mirror Sphere", new Material("Mirror", vec4_t{ 1.0, 1.0, 1.0, 0 }, vec4_t{ 1.0, 1.0, 1.0, 0 }, vec4_t{ 1.0, 1.0, 1.0, 0 }, 200.0, 1.0, 0.8), new Sphere(vec4_t{0.0,0.0,-2.0,0.0}, 0.8)));
    obj_list.push_back(new Object("Refract Sphere", new Material("Refract", vec4_t{ 1.0, 1.0, 1.0, 0 }, vec4_t{ 1.0, 1.0, 1.0, 0 }, vec4_t{ 0.2, 0.2, 0.2, 0 }, 2, 0.5, 0.5, 1.1), new Sphere(vec4_t{0.0,0.0,2.0,0.0}, 0.8)));
    
    //light_list.push_back(new AmbientLight(vec4_t{1.0,1.0,1.0,1.0}));
    light_list.push_back(new DirectionalLight(vec4_t{ 1.0, 1.0, 1.0, 0.0 }, vec4_t{ 1.0, 1.0, 1.0, 0.0 }));
    light_list.push_back(new DirectionalLight(vec4_t{ 1.0, 1.0, 1.0, 0.0 }, vec4_t{ -1.0, 1.0, 1.0, 0.0 }));

    render(cam, obj_list, light_list, buffer);

    //Generate PPM Image
    cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            size_t pixel_index = j*nx + i;
            int ir = int(255.99*buffer[pixel_index].r);
            int ig = int(255.99*buffer[pixel_index].g);
            int ib = int(255.99*buffer[pixel_index].b);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }


    return 0;
}

vec4_t raycast (ray_t ray, const vector<Object *> &obj_list, const vector<Light *> &light_list, float near, int depth){
    float dist = INFINITY;
    collision_t closest_collision;
    vec4_t accucolor = vec4_t{ 0.8, 0.8, 0.8, 0.0 }; //white room
    //vec4_t accucolor = vec4_t{ 0.5, 0.5, 0.5, 0.0 }; //Medium room
    const Object *closest_obj = nullptr;

    for (Object * obj : obj_list) {
        const collision_t col = obj->geometry->CollideClosest(ray);//Collision
        const float o_dist = col.distance;
        if (o_dist < dist && o_dist > near) {
            dist = o_dist;
            closest_collision = col;
            closest_obj = obj;
        }
    }


    if (closest_obj){
        //accucolor += vec4_t{1.0,1.0,1.0,0.0};
        //accucolor += closest_obj->material->ka;
        //cout << "Object: " << closest_obj->name << endl;
        Material mat = *closest_obj->material;
        vec4_t normal = closest_obj->geometry->getNormal(closest_collision.pint);
        accucolor = mat.ka;
        
        for (Light * light : light_list) {
            const vec4_t direction = light->GetLightDirection(closest_collision.pint);
            const float dot_d_n = dot(direction, normal);
            
            if (dot_d_n >= 0){
                accucolor += light->Illumination(mat, normal, ray.direction * (-1.0), closest_collision.pint)*mat.alfa;
                
                /*
                const vec4_t L_from_source = light->GetLightDirection(ray.origin);
                float d = light->LightDistance(ray.origin);
                float attenuation = 0.1f * d;
                float dot_L_v = dot(L_from_source, ray.direction);
                if (dot_L_v > 0.0f){
                    accucolor += light->intensity * 4.0f * powf(dot_L_v, 5000.0f) / attenuation;
                }
                */

            }
        }

        
        if (mat.reflection && depth < 3) {
            const vec4_t reflect = (reflection(ray.direction * (-1.0), normal)).unit();
            accucolor += raycast(ray_t{ closest_collision.pint, reflect}, obj_list, light_list, 0.00001, depth+1) * mat.reflection;
        }

        if (mat.alfa < 1 && depth < 3) {
            vec4_t vec_offset = closest_collision.pint + ray.direction * 0.001; //Avoid bugs
            const vec4_t refract_ray = refract(ray.direction, normal, mat.ior, 1);
            accucolor += raycast(ray_t{ vec_offset , refract_ray }, obj_list, light_list, 0.00001, depth + 1)*(1-mat.alfa);
        }

        accucolor = accucolor * 0.5;
    }

    return accucolor;

}

void render (const camera_t &cam, const vector<Object *> &obj_list, const vector<Light *> &light_list, vec4_t * buffer) {

    
    const vec4 base = (cam.lookat - cam.position).unit();
    const vec4 up = cam.up * (cam.dimensions.y / cam.resolution.y);
    const vec4 left = cross(cam.up, base) * (cam.dimensions.x / cam.resolution.x);

    const int width  = (int)cam.resolution.x;
    const int height = (int)cam.resolution.y;

    const vec4_t origin_screen = base * cam.n + up * (height*(-0.5) + 0.5) + left * (width*(0.5) - 0.5);

    //Scan screen
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            vec4_t direction = (origin_screen + up * (y) + left * (-x)).unit();

            buffer[y*width + x] = raycast(ray_t{cam.position, direction}, obj_list, light_list, cam.n, 0);

        }
    }

}
