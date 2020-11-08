//
// Created by diego on 8/11/20.
//

#include "../p1/Sist_coord.hpp"
#include "../p1/Punto.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

#ifndef P3_CAMERA_HPP
#define P3_CAMERA_HPP


class Camera {
    Sist_coord sistCoordCam;
    Punto origen;
public:
    Camera(const Sist_coord &sistCoordCam, const Punto &origen);
};

class Rayo{
    Punto origen;
    Vector direccion;
public:
    Rayo(const Punto &origen, const Vector &direccion);
    double interseccion(Plane plano);
    double interseccion(Sphere esfera);
    double secGradeEcuation(double a, double b, double c);
};

#endif //P3_CAMERA_HPP
