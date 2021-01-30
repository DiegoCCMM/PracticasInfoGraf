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
    bool hayEvento;

public:

    Rayo() : origen(Punto()), direccion(Vector()), hayEvento(true) {}

    Rayo(const Punto &origen, const Vector &direccion) : 
                origen(origen), direccion(direccion), hayEvento(true) {}

    Vector getDir ()   { return direccion; }
    Punto getOrigen () { return origen;    }
    void setEvento(bool val){
        hayEvento = val;
    }
    bool hayEvento(){
        return hayEvento;
    }

};

#endif //P3_CAMERA_HPP
