//
// Created by diego on 8/11/20.
//

#include "../p1/Vector.hpp"
#include "../p1/Punto.hpp"
#include "Camera.hpp"
#include "geometryRGBFigures.hpp"

#ifndef P3_PLANE_HPP
#define P3_PLANE_HPP


class Plane : public geometryRGBFigures{

    Vector normal;
    Punto punto;
    double distance;

public:

    Plane(Vector normal, Punto punto, int red, int green, int blue){
        this->normal = normal;
        this->punto = punto;
        this->distance = punto*normal;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    Plane(Vector normal, double distance, int red, int green, int blue){
        this->normal = normal;
        this->distance = distance;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    Vector getNormal(Punto inters) override { return normal; }

    void setNormal(const Vector &normal){ this->normal = normal; }

    double getDistance() const{  return distance; }

    void setDistance(double distance){ this->distance = distance; }

    double interseccion(Rayo ray) override {
        double c = distance;
        Punto o = ray.getOrigen();
        Vector  d = ray.getDir(),
                n = normal;
        // Return t
        return -(c + o*n) / (d*n);
    }

    Matriz ejeCoord (Rayo ray) {
        Vector j = ray.getDir()->*normal;
        Vector k = j->*normal;

        Matriz resul(normal, j, k);

        return resul;
    }

};


#endif //P3_PLANE_HPP