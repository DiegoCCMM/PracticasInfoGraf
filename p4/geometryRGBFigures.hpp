//
// Created by diego on 11/11/20.
//

#include "Camera.hpp"

#ifndef P3_GEOMETRYRGBFIGURES_HPP
#define P3_GEOMETRYRGBFIGURES_HPP


class geometryRGBFigures {

public:

    int red, green, blue;

    int getRed()   const { return red;   }
    int getGreen() const { return green; }
    int getBlue()  const { return blue;  }

    virtual double interseccion (Rayo ray) {}
    
};


#endif //P3_GEOMETRYRGBFIGURES_HPP
