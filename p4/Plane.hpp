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

    Plane(Vector normal, Punto punto, int red = 0, int green = 0, int blue = 0){
        this->normal = normal;
        this->punto = punto;
        this->distance = -(punto*normal);
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    Vector getNormal(Punto inters) override { 
        return normal.normalizar();
    }

    void setNormal(const Vector &normal){ this->normal = normal; }

    double getDistance() const{  return distance; }

    void setDistance(double distance){ this->distance = distance; }

    // cambiar
    double interseccion(const Rayo &ray) const override {
        double c = distance;
        Punto o = ray.getOrigen();
        Vector  d = ray.getDir(),
                n = normal;

        if(d*n >= 0){
            return -1.0;
        }

        // Return t
        // Si t negativo --> no intersecta
        // si d*n == 0 --> no intersecta (en infinito)

        return -(c + o*n) / (d*n);
    }

    double getFacingRatio(Rayo rayo) override {
        return (getNormal(rayo.getOrigen()) * rayo.getDir());
    }

};


#endif //P3_PLANE_HPP