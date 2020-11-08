//
// Created by diego on 8/11/20.
//

#include "Sphere.hpp"

Sphere::Sphere(Punto center, double radius, int red, int green, int blue) {
    this->center = center;
    this->radius = radius;
    red = red;
    green = green;
    blue = blue;
}

const Punto &Sphere::getCenter() const {
    return center;
}

void Sphere::setCenter(const Punto &center) {
    Sphere::center = center;
}

 double Sphere::getRadius() const {
    return radius;
}

void Sphere::setRadius(double radius) {
    Sphere::radius = radius;
}
