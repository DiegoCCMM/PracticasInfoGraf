#include "../p1/Vector.hpp"
#include "../p1/Punto.hpp"
#include "Camera.hpp"
#include "geometryRGBFigures.hpp"
#include "math.h"


#ifndef P4_SPHERE_HPP
#define P4_SPHERE_HPP


class FocoPuntual {

    int red, green, blue;
    Punto position = Punto(0,0,0);

public:

    FocoPuntual(){}
    FocoPuntual(Punto position, int red = 0, int green = 0, int blue = 0){
        this->position = position;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    Punto getPosition() const { return position; }
    void setPosition(const Punto &position){ this->position = position; }

    int getRed()   const { return red;   }
    int getGreen() const { return green; }
    int getBlue()  const { return blue;  }

    RGB getRGB() const {
        return RGB(red/255.0, green/255.0, blue/255.0);
    }

private:
    
};


#endif //P4_SPHERE_HPP
