//
// Created by diego on 8/11/20.
//

#include "Plane.hpp"

Plane::Plane(Vector normal, double distance, int red, int green, int blue) {
     normal = normal;
     distance = distance;
     red = red;
     green = green;
     blue = blue;
}

const Vector Plane::getNormal() const {
    return normal;
}

void Plane::setNormal(const Vector &normal) {
    Plane::normal = normal;
}

double Plane::getDistance() const {
    return distance;
}

void Plane::setDistance(double distance) {
    Plane::distance = distance;
}

