#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));  // Chão
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));  
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));    
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));  
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));   

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;  
    cam.image_width       = 400;         
    cam.samples_per_pixel = 100;         
    cam.max_depth         = 50;          

    cam.vfov     = 40;                     
    cam.lookfrom = point3(-2, 2, 1);       
    cam.lookat   = point3(0, 0, -1);       
    cam.vup      = vec3(0, 1, 0);          

    cam.defocus_angle = 10.0;  
    cam.focus_dist    = 3.4;

    cam.render(world); 
}

// Configuração mais leve, mas ainda sim renderiza uma cena mais complexa, ideal para computadores medianos.
// Se seu computador for bom, em vez de rodar "g++ main.cpp -o main && main.exe > imagem.ppm", rode "g++ main2.cpp -o main2 && main2.exe > imagem.ppm"