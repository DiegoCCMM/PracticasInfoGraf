//
// Created by diego on 3/10/20.
//


#include <cmath>
#include "Figures_3D.hpp"

#ifndef PRACTICAS_VECTOR_HPP
#define PRACTICAS_VECTOR_HPP

class Vector : public Figures_3D {
public:
    Vector(double coordx, double coordy, double coordz) {
        x = coordx;
        y = coordy;
        z = coordz;
    }

    double module() {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    Vector productoVectorial(Vector a, Vector b){

        return {a.y * b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
    }

    Vector operator+(const Vector b) {
        return {this->x + b.x, this->y + b.y, this->z + b.z};
    }

    Vector operator-(const Vector b) {
        return {this->x - b.x, this->y - b.y, this->z - b.z};
    }

    double operator*(const Vector b) {  //producto escalar
        return this->x * b.x + this->y * b.y + this->z * b.z;
    }

    Vector operator/(const Vector b) {
        return {this->x / b.x, this->y / b.y, this->z / b.z};
    }

    Vector operator*(const int b) {
        return {this->x * b, this->y * b, this->z * b};
    }
    
    Vector operator/(const int b) {
        return {this->x / b, this->y / b, this->z / b};
    }

};

#endif //PRACTICAS_VECTOR_HPP
