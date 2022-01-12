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

    Sphere(){}
    Sphere(Punto center, double radius, int red = 0, int green = 0, int blue = 0){
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

    double interseccion(const Rayo &ray) const override {

        Punto  o = ray.getOrigen(),
               c = center;
        Vector d = ray.getDir();
        double r = radius;

        double  x = pow((d.module()), 2),
                y = d.mul(2) * (o-c),
                z = pow(((o-c).module()), 2) - pow(r, 2);

        return secGradeEcuation(x, y, z);
    }

    Vector getNormal(Punto inters) override { 
        return (inters - this->center).normalizar(); 
    }

    double getFacingRatio(Rayo rayo) override {
        return (((rayo.getOrigen() - this->center).normalizar()) * rayo.getDir());
    }

private:

    double secGradeEcuation(double a, double b, double c) const {
        if(pow(b,2) - 4 * a * c >= 0) {

            double primeraSol = (-b - sqrt(pow(b,2) - 4 * a * c)) / (2*a);
            double segundaSol = (-b + sqrt(pow(b,2) - 4 * a * c)) / (2*a);

            if (primeraSol >= 10^(-3) && segundaSol < 10^(-3)){ return primeraSol;}
            else if (primeraSol < 10^(-3) && segundaSol >= 10^(-3)){ return segundaSol;}
            else {
                if(primeraSol< 10^(-3)) return -1.0; //las dos son negativas
                //las dos son positivas, cogemos el minimo
                else return min(primeraSol, segundaSol);
            }

        }
        return -1;
    }
    
};


#endif //P3_SPHERE_HPP
