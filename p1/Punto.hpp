//
// Created by diego on 3/10/20.
//


#include <cmath>
#include "Figures_3D.hpp"

#ifndef PRACTICAS_PUNTO_HPP
#define PRACTICAS_PUNTO_HPP
//
// Created by diego on 3/10/20.
//

class Punto : public Figures_3D {
public:
    Punto(double coordx, double coordy, double coordz) {
        x = coordx;
        y = coordy;
        z = coordz;
    }

    double module() {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    Vector operator-(const Punto b) {
        return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
    }

    Punto operator*(const Punto b) {
        return {this->x * b.x, this->y * b.y, this->z * b.z};
    }

    Punto operator/(const Punto b) {
        return {this->x / b.x, this->y / b.y, this->z / b.z};
    }

};

#endif //PRACTICAS_PUNTO_HPP
