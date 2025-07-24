#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

// Classe base abstrata para materiais que definem como os raios interagem com as superfícies
class material {
  public:
    virtual ~material() = default;

    // Determina como um raio é espalhado ao atingir uma superfície
    // Retorna true se o raio foi espalhado, false se foi absorvido
    // r_in: raio incidente
    // rec: registro de interseção contendo informações sobre o ponto de interseção
    // attenuation: cor atenuada do raio espalhado
    // scattered: raio espalhado resultante
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;  // Material padrão não espalha luz (absorve tudo)
    }
};

// Material difuso que espalha a luz igualmente em todas as direções (superfície fosca)
class lambertian : public material {
    public:
      // albedo: cor base do material (refletância difusa)
      lambertian(const color& albedo) : albedo(albedo) {}
  
      bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
      const override {
          // Gera uma direção de espalhamento aleatória em um hemisfério centrado na normal
          auto scatter_direction = rec.normal + random_unit_vector();
  
          // Evita direção de espalhamento degenerada (vetor zero)
          if (scatter_direction.near_zero())
              scatter_direction = rec.normal;
  
          // Cria um novo raio espalhado a partir do ponto de interseção
          scattered = ray(rec.p, scatter_direction);
          attenuation = albedo;  // A cor é atenuada pelo albedo do material
          return true;  // Sempre espalha a luz
      }
  
    private:
      color albedo;  // Cor base do material
};

// Material metálico que reflete a luz como um espelho (pode ter rugosidade)
class metal : public material {
  public:
    // albedo: cor do material
    // fuzz: fator de rugosidade (0 = espelho perfeito, 1 = superfície muito rugosa)
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        // Calcula a direção refletida
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        // Adiciona rugosidade à direção refletida
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        // Verifica se o raio refletido está acima da superfície
        return (dot(scattered.direction(), rec.normal) > 0);
    }

  private:
    color albedo;  // Cor do material
    double fuzz;   // Fator de rugosidade (0 a 1)
};

// Material dielétrico que refrata a luz (como vidro, água, etc.)
class dielectric : public material {
  public:
    // refraction_index: índice de refração do material (ex: 1.5 para vidro)
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        attenuation = color(1.0, 1.0, 1.0);  // Os dielétricos não absorvem luz
        
        // Determina o índice de refração relativo (ar->material ou material->ar)
        double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        // Verifica se ocorre reflexão interna total
        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        // Usa a reflexão ou refração baseado no ângulo crítico e no efeito de Fresnel
        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);  // Reflexão
        else
            direction = refract(unit_direction, rec.normal, ri);  // Refração

        scattered = ray(rec.p, direction);
        return true;
    }

  private:
    double refraction_index;  // Índice de refração do material

    // Aproximação de Schlick para o coeficiente de reflexão de Fresnel
    // Usado para calcular a probabilidade de reflexão em diferentes ângulos
    static double reflectance(double cosine, double refraction_index) {
        // Fórmula de Schlick para reflexão de Fresnel
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
};

#endif