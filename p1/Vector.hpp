//
// Created by diego on 3/10/20.
//


#include <cmath>
#include "Figures_3D.hpp"

#ifndef PRACTICAS_VECTOR_HPP
#define PRACTICAS_VECTOR_HPP

class Vector : public Figures_3D {
public:
	Vector(){}

    Vector(double coordx, double coordy, double coordz) {
        x = coordx;
        y = coordy;
        z = coordz;
    }

    void setZ(double coordz){
        z = coordz;
    }

    double module() {
        return sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
    }

    Vector operator+(const Vector b) {
        return {this->x + b.x, this->y + b.y, this->z + b.z};
    }

    Vector operator-(const Vector b) {
        return {this->x - b.x, this->y - b.y, this->z - b.z};
    }

    /* Producto escalar */
    double operator*(const Vector b) {
        return this->x * b.x + this->y * b.y + this->z * b.z;
    }

    /* Producto vectorial */
    Vector operator->*(const Vector b) {
		return {this->y*b.z - this->z*b.y,
				this->z*b.x - this->x*b.z,
				this->x*b.y - this->y*b.x};
	}
    
    Vector operator/(const double b) {
        return Vector(this->x / b, this->y / b, this->z / b);
    }

    Vector mul(const double a) const {
        return {this->x*a, this->y*a, this->z*a};
    }

    Vector div(const double a) {
        return {this->x/a, this->y/a, this->z/a};
    }

    Vector sinV() {
        return {sin(this->x), sin(this->y), sin(this->z)};
    }

    Vector asinV() {
        return {asin(this->x), asin(this->y), asin(this->z)};
    }

    Vector normalizar() {
        Vector v(this->x, this->y, this->z);
        return v/v.module();
    }

};

#endif //PRACTICAS_VECTOR_HPP
