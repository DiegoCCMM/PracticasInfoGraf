//
// Created by diego on 8/11/20.
//

#include "../p1/Figures_3D.hpp"
#include "../p1/Vector.hpp"
#include "../p1/Punto.hpp"



#ifndef P3_SPHERE_HPP
#define P3_SPHERE_HPP


class Sphere {

    Punto center = Punto(0, 0, 0);
    double radius;
public:
    const Punto &getCenter() const;

    void setCenter(const Punto &center);

    double getRadius() const;

    void setRadius(double radius);

private:
    int red;
    int green;
    int blue;

public:
    Sphere(Punto center, double radius, int red, int green, int blue);
};


#endif //P3_SPHERE_HPP
