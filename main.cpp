#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
    hittable_list world;

    // chão
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // esfera de vidro (central)
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    // Esfera Marrom Fosca (à esquerda)
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-2.5, 1, 0), 1.0, material2));

    // Esfera de Metal (à direita)
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(2.5, 1, 0), 1.0, material3));

    // Pequenas esferas adicionais para enriquecer a cena
    world.add(make_shared<sphere>(point3(1.5, 0.6, 1.5), 0.6, make_shared<metal>(color(0.8, 0.2, 0.2), 0.4)));
    world.add(make_shared<sphere>(point3(-1.5, 0.6, 1.5), 0.6, make_shared<lambertian>(color(0.2, 0.8, 0.2))));
    world.add(make_shared<sphere>(point3(0.0, 0.4, 2.0), 0.4, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(point3(-0.8, 0.3, 2.8), 0.3, make_shared<metal>(color(0.2, 0.2, 0.8), 0.8)));
    world.add(make_shared<sphere>(point3(0.8, 0.3, 2.8), 0.3, make_shared<lambertian>(color(0.8, 0.8, 0.2))));

    // Configurações de câmera e renderização
    camera cam;

    // Configurações de renderização otimizadas para desempenho
    cam.aspect_ratio      = 16.0 / 9.0;  // Proporção de tela 16:9
    cam.image_width       = 400;         // Largura menor para renderização mais rápida
    cam.samples_per_pixel = 100;         // Menos amostras por pixel para melhor desempenho
    cam.max_depth         = 50;          // Profundidade máxima de raios para reflexos

    // Posicionamento e orientação da câmera
    cam.vfov     = 20;  // Campo de visão vertical em graus
    cam.lookfrom = point3(13, 2.5, 4);  // Posição da câmera no espaço 3D
    cam.lookat   = point3(0, 0.7, 0);   // Ponto para onde a câmera está olhando
    cam.vup      = vec3(0,1,0);         // Vetor para cima da câmera

    // Configurações de desfoque (profundidade de campo)
    cam.defocus_angle = 0.6;  // Ângulo de desfoque em graus
    cam.focus_dist    = 10.0; // Distância de foco

    // Iniciar o processo de renderização
    cam.render(world);
}


// Esse código para o render é mais básico e renderiza apenas 9 esferas, para o meu computador que é mais antiguinho eu tive que rodar ele, pois ele não aguenta a configuração
//aplicada no livro.