#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "rtweekend.h"


#include <vector>


// Classe que representa uma coleção de objetos que podem ser atingidos por raios
class hittable_list : public hittable {
  public:
    // Lista de objetos que podem ser atingidos
    std::vector<shared_ptr<hittable>> objects;

    // Construtores
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    // Limpa a lista de objetos
    void clear() { objects.clear(); }

    // Adiciona um objeto à lista
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    // Verifica se algum objeto na lista é atingido pelo raio
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;  // Acompanha a interseção mais próxima

        // Verifica interseção com cada objeto na lista
        for (const auto& object : objects) {
            // Usa um intervalo que vai do mínimo até a interseção mais próxima até agora
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;  // Atualiza a interseção mais próxima
                rec = temp_rec;               // Salva o registro de interseção
            }
        }

        return hit_anything;
    }
};

#endif