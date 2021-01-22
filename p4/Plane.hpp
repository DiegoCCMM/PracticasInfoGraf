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
    double distance;

public:

    Plane(Vector normal, double distance, int red, int green, int blue){
        this->normal = normal;
        this->distance = distance;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    const Vector getNormal() const{ return normal; }

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

};


#endif //P3_PLANE_HPP
