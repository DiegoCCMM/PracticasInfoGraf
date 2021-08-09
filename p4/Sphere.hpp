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

        double  x = pow((d.module()), 2),
                y = d.mul(2) * (o-c),
                z = pow(((o-c).module()), 2) - pow(r, 2);

        return secGradeEcuation(x, y, z);
    }

    Vector getNormal(Punto inters) override { 
        return (inters - this->center); 
        // return (inters - this->center)/(inters - this->center).module(); 
    }

    Matriz ejeCoord (Rayo ray, Punto inters) override {
        // Y falta normalizar
        // Vector normal = Punto(radius+center.getX(),center.getY(),center.getZ()) - this->center;
        // normal = normal/normal.module();
        // normal.normalizar();
        Vector normal = this->getNormal(inters);
        Vector i = ray.getDir() ->* normal;
        i.normalizar();
        Vector j = i ->* normal;
        j.normalizar();
        // Comprobar que ningun producto vectorial el resultado es 0

        Matriz resul(i, j, normal.normalizar());

        return(resul);
    }

private:

    double secGradeEcuation(double a, double b, double c) {
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
