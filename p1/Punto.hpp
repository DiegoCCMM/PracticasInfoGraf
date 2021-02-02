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
    Punto(){}

    Punto(double coordx, double coordy, double coordz) {
        x = coordx;
        y = coordy;
        z = coordz;
    }

    double getX(){
        return x;
    }

    double getY(){
        return y;
    }

    double getZ(){
        return z;
    }

    double module() {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    Vector operator-(const Punto b) {
        return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
    }

    /* Producto escalar */
    double operator*(const Vector b) {
        return this->x * b.x + this->y * b.y + this->z * b.z;
    }

    Punto operator+(const Vector b) {
        return Punto(this->x + b.x, this->y + b.y, this->z + b.z);
    }

    Vector sum(const Vector b) {
        return Vector(this->x + b.x, this->y + b.y, this->z + b.z);
    }

    Vector minus(const Vector b) {
        return Vector(b.x - this->x, b.y - this->y, b.z - this->z);
    }
};

#endif //PRACTICAS_PUNTO_HPP
