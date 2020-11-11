//
// Created by diego on 8/11/20.
//

#include "../p1/Figures_3D.hpp"
#include "../p1/Vector.hpp"
#include "../p1/Punto.hpp"
#include "geometryRGBFigures.hpp"

#ifndef P3_PLANE_HPP
#define P3_PLANE_HPP


class Plane : public geometryRGBFigures{

    Vector normal;
    double distance;
public:
    const Vector getNormal() const;

    void setNormal(const Vector &normal);

    double getDistance() const;

    void setDistance(double distance);


public:
    Plane(Vector normal, double distance, int red, int green, int blue);


};


#endif //P3_PLANE_HPP