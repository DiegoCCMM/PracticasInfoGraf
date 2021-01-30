//
// Created by diego on 11/11/20.
//

#include "Camera.hpp"
#include <cmath>

#ifndef P3_GEOMETRYRGBFIGURES_HPP
#define P3_GEOMETRYRGBFIGURES_HPP


class geometryRGBFigures {

public:

    int red, green, blue;

    double kd, ks, kt;

    int getRed()   const { return red;   }
    int getGreen() const { return green; }
    int getBlue()  const { return blue;  }

    virtual double interseccion (Rayo ray) {}
    virtual Matriz ejeCoord (Rayo ray) {}

    void esDifuso(){
        kd = max(max(red, green), blue)/(double)(red + green + blue);
    }

    void esDielectrico(){
        ks = 0.4;
        kt = 0.6;
    }

    double getKd() const {
        return kd;
    }

    double getKs() const {
        return ks;
    }

    double getKt() const {
        return kt;
    }

    void esEspecular(){
        kd = max(max(red, green), blue)/(double)(red + green + blue);
        ks = 1-kd;
    }
    
};


#endif //P3_GEOMETRYRGBFIGURES_HPP
