//
// Created by diego on 8/11/20.
//

#include "../p1/Vector.hpp"
#include "../p1/Punto.hpp"
#include "Camera.hpp"
#include "geometryRGBFigures.hpp"
#include "math.h"


#ifndef P3_SPHERE_HPP
#define P3_SPHERE_HPP


class Sphere : public geometryRGBFigures {

    Punto center = Punto(0,0,0);
    double radius;

public:

    Sphere(Punto center, double radius, int red, int green, int blue){
        this->center = center;
        this->radius = radius;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    const Punto &getCenter() const{ return center; }
    void setCenter(const Punto &center){ this->center = center; }

    double getRadius() const{ return radius; }
    void setRadius(double radius){ this->radius = radius; }

    double interseccion(Rayo ray) override {

        Punto  o = ray.getOrigen(),
               c = center;
        Vector d = ray.getDir();
        double r = radius;

        double  x = pow(d.module(), 2),
                y = 2 * (d * (o-c)),
                z = pow((o-c).module(), 2) - pow(r, 2);

        return secGradeEcuation(x, y, z);
    }

private:

    double secGradeEcuation(double a, double b, double c) {
        if(pow(b,2) - 4 * a * c >= 0) {

            double primeraSol = (-b - sqrt(pow(b,2) - 4 * a * c)) / (2*a);
            double segundaSol = (-b + sqrt(pow(b,2) - 4 * a * c)) / (2*a);

            if (primeraSol < segundaSol) return primeraSol;
            else return segundaSol;
        }
        return -1;
    }
    
};


#endif //P3_SPHERE_HPP
