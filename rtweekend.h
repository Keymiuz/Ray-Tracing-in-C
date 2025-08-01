#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>


using std::make_shared;
using std::shared_ptr;

// Constantes

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Funções Utilitárias

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Retorna um número real aleatório em [0,1).
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Retorna um número real aleatório em [min,max).
    return min + (max-min)*random_double();
}

#endif