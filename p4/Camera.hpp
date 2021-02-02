//
// Created by diego on 8/11/20.
//

#include "../p1/Sist_coord.hpp"
#include "../p1/Matriz.hpp"
#include "../p1/Punto.hpp"

#ifndef P3_CAMERA_HPP
#define P3_CAMERA_HPP


class Camera {

    Matriz sistCoordCam;
    Punto origen;

public:

    Camera(const Matriz &sistCoordCam, const Punto &origen) :
                sistCoordCam(sistCoordCam), origen(origen) {}

};

class Rayo {

    Punto origen;
    Vector direccion;
    double absorcion;

public:

    Rayo() : origen(Punto()), direccion(Vector()), absorcion(0.0) {}

    Rayo(const Punto &origen, const Vector &direccion) : 
                origen(origen), direccion(direccion), absorcion(0.0) {}

    Vector getDir ()   { return direccion; }
    Punto getOrigen () { return origen;    }

    void setAbsorcion(double val){
        absorcion = val;
    }

    double getAbsorcion(){
        return absorcion;
    }

    bool hayAbsorcion() const {
        return absorcion==1.0;
    }

};

#endif //P3_CAMERA_HPP
