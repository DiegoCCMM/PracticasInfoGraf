//
// Created by diego on 7/11/20.
//

#include <string>
#include "Plane.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"


int main(int argc, char* argv[]){

    std::string s = argv[1];
    int pixelRes = stoi(s);

    //primera escena
    //
    //Esfera 0,0,10 / 10
    //Cámara 0,0,0 / y = 0,15,0 – z = 0,0,10 – x = y X z
    //Pixeles 144

    Sist_coord cam = Sist_coord(Vector(0,0,10),Vector(0,10,0));

    Camera camera = Camera(cam, Punto(0,0,0));

    camera.

}