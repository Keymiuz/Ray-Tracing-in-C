#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "ray.h"
#include "interval.h"

// Declaração antecipada da classe material
class material;

// Estrutura que armazena informações sobre uma interseção raio-objeto
class hit_record {
  public:
    point3 p;                       // Ponto de interseção
    vec3 normal;                    // Vetor normal no ponto de interseção
    shared_ptr<material> mat;       // Material do objeto atingido
    double t;                       // Parâmetro t do raio no ponto de interseção
    bool front_face;                // Se a face atingida é a frontal (true) ou traseira (false)

    // Define a normal da superfície baseada no lado atingido pelo raio
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Se o produto escalar for negativo, o raio está vindo de fora
        front_face = dot(r.direction(), outward_normal) < 0;
        // A normal sempre aponta contra o raio incidente
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// Classe base abstrata para todos os objetos que podem ser atingidos por raios
class hittable {
  public:
    // Destrutor virtual para permitir herança adequada
    virtual ~hittable() = default;

    // Verifica se um raio atinge o objeto dentro de um intervalo de parâmetros
    // Retorna true se houver interseção e preenche o registro de interseção
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif