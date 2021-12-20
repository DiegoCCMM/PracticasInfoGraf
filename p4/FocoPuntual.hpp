#include "../p1/Vector.hpp"
#include "../p1/Punto.hpp"
#include "Camera.hpp"
#include "geometryRGBFigures.hpp"
#include "math.h"


#ifndef P4_SPHERE_HPP
#define P4_SPHERE_HPP


class FocoPuntual : public geometryRGBFigures {

    Punto position = Punto(0,0,0);
    int intensidad = 1;

public:

    FocoPuntual(){}
    FocoPuntual(Punto position, int red = 0, int green = 0, int blue = 0){
        this->position = position;
        this->red = red;
        this->green = green;
        this->blue = blue;
        foco=true;
        this->esDifuso();
    }

    Punto getPosition() const { return position; }
    void setPosition(const Punto &position){ this->position = position; }

    int getIntensidad() const { return intensidad; }
    void setIntensidad(const int &intensidad) { this->intensidad = intensidad; }

private:
    
};


#endif //P4_SPHERE_HPP
