#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "material.h"


class camera {
    public:
      // Razão entre largura e altura da imagem
      double aspect_ratio      = 1.0;  // Razão entre largura e altura da imagem
      // Largura da imagem em pixels
      int    image_width       = 100;  // Largura da imagem em pixels
      // Número de amostras por pixel para anti-aliasing
      int    samples_per_pixel = 10;   // Número de amostras por pixel para anti-aliasing
      // Número máximo de reflexões/refrações
      int    max_depth         = 10;   // Número máximo de reflexões/refrações
      // Ângulo de visão vertical (campo de visão)
      double vfov = 90;  // Ângulo de visão vertical (campo de visão)
      // Ponto de origem da câmera
      point3 lookfrom = point3(0,0,0);   // Ponto de origem da câmera
      // Ponto para o qual a câmera está olhando
      point3 lookat   = point3(0,0,-1);  // Ponto para o qual a câmera está olhando
      // Vetor "para cima" da câmera
      vec3   vup      = vec3(0,1,0);     // Vetor "para cima" da câmera

      // Ângulo de desfoque para profundidade de campo
      double defocus_angle = 0;  // Ângulo de desfoque para profundidade de campo
      // Distância para o plano de foco perfeito
      double focus_dist = 10;    // Distância para o plano de foco perfeito


      // Renderiza a cena
      void render(const hittable& world) {
          initialize();
  
          // Cabeçalho do arquivo PPM
          std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  
          // Loop através de cada linha de pixels
          for (int j = 0; j < image_height; j++) {
              // Mostra progresso
              std::clog << "\rLinhas restantes: " << (image_height - j) << ' ' << std::flush;
              
              // Loop através de cada coluna de pixels
              for (int i = 0; i < image_width; i++) {
                  color pixel_color(0,0,0);
                  // Amostragem por pixel para anti-aliasing
                  for (int sample = 0; sample < samples_per_pixel; sample++) {
                      ray r = get_ray(i, j);
                      pixel_color += ray_color(r, max_depth, world);
                    }
                  // Escreve a cor média das amostras
                  write_color(std::cout, pixel_samples_scale * pixel_color);
              }
          }
  
          std::clog << "\rConcluído.                 \n";
      }
  
    private:
      // Altura da imagem renderizada
      int    image_height;         // Altura da imagem renderizada
      // Fator de escala para a média das amostras de cor
      double pixel_samples_scale;  // Fator de escala para a média das amostras de cor
      // Centro da câmera
      point3 center;               // Centro da câmera
      // Localização do pixel (0,0)
      point3 pixel00_loc;          // Localização do pixel (0,0)
      // Vetor para o próximo pixel à direita
      vec3   pixel_delta_u;        // Vetor para o próximo pixel à direita
      // Vetor para o próximo pixel abaixo
      vec3   pixel_delta_v;        // Vetor para o próximo pixel abaixo
      // Vetores de base u,v,w para o sistema de coordenadas da câmera
      vec3   u, v, w;              // Vetores de base u,v,w para o sistema de coordenadas da câmera
      // Vetores para o disco de desfoque (profundidade de campo)
      vec3   defocus_disk_u;       // Vetores para o disco de desfoque (profundidade de campo)
      vec3   defocus_disk_v;       // Vetores para o disco de desfoque (profundidade de campo)


      // Calcula a cor do raio, incluindo reflexões e refrações
      color ray_color(const ray& r, int depth, const hittable& world) const {
          // Se excedeu o número máximo de reflexões, não contribui mais com luz
          if (depth <= 0)
              return color(0,0,0);

          hit_record rec;

          // Se o raio atingir um objeto
          if (world.hit(r, interval(0.001, infinity), rec)) {
              ray scattered;
              color attenuation;
              // Se o material espalhar o raio, continue o caminho do raio
              if (rec.mat->scatter(r, rec, attenuation, scattered))
                  return attenuation * ray_color(scattered, depth-1, world);
              return color(0,0,0);
          }

          // Gradiente de fundo (céu)
          vec3 unit_direction = unit_vector(r.direction());
          auto a = 0.5*(unit_direction.y() + 1.0);
          return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
      }

      // Inicializa os parâmetros da câmera
      void initialize() {
          // Calcula a altura da imagem com base na largura e na proporção
          image_height = int(image_width / aspect_ratio);
          image_height = (image_height < 1) ? 1 : image_height;
      
          // Fator de escala para a média das amostras
          pixel_samples_scale = 1.0 / samples_per_pixel;
      
          // Define o centro da câmera
          center = lookfrom;
      
          // Determina as dimensões da viewport
          auto theta = degrees_to_radians(vfov);
          auto h = std::tan(theta/2);
          auto viewport_height = 2 * h * focus_dist;
          auto viewport_width = viewport_height * (double(image_width)/image_height);
      
          // Calcula os vetores de base u,v,w para o sistema de coordenadas da câmera
          w = unit_vector(lookfrom - lookat);
          u = unit_vector(cross(vup, w));
          v = cross(w, u);
      
          // Vetores que percorrem a viewport
          vec3 viewport_u = viewport_width * u;    // Vetor horizontal da viewport
          vec3 viewport_v = viewport_height * -v;  // Vetor vertical da viewport (invertido)
      
          // Delta para mover de um pixel para outro
          pixel_delta_u = viewport_u / image_width;
          pixel_delta_v = viewport_v / image_height;
      
          // Localização do canto superior esquerdo
          auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
          pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

          // Calcula os vetores do disco de desfoque para profundidade de campo
          auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
          defocus_disk_u = u * defocus_radius;
          defocus_disk_v = v * defocus_radius;
      }
    
      // Obtém um raio para a posição de pixel (i,j)
      ray get_ray(int i, int j) const {
          // Obtém uma amostra aleatória dentro do pixel
          auto offset = sample_square();
          auto pixel_sample = pixel00_loc
                            + ((i + offset.x()) * pixel_delta_u)
                            + ((j + offset.y()) * pixel_delta_v);

          // Origem do raio (com desfoque para profundidade de campo)
          auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
          auto ray_direction = pixel_sample - ray_origin;

          return ray(ray_origin, ray_direction);
      }

      // Retorna um ponto aleatório no quadrado unitário [-0.5,-0.5] a [+0.5,+0.5]
      vec3 sample_square() const {
          return vec3(random_double() - 0.5, random_double() - 0.5, 0);
      }

      // Retorna um ponto aleatório no disco de desfoque da câmera
      point3 defocus_disk_sample() const {
          auto p = random_in_unit_disk();
          return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
      }
};

#endif