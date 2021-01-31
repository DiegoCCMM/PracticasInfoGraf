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

    double kd=0.0, ks=0.0, kt=0.0;

    bool foco = false;

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

    void esEspecular(){
        kd = max(max(red, green), blue)/(double)(red + green + blue);
        ks = 1-kd;
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

    bool soyFoco() const {
        return foco;
    }

    void setFoco(bool val) {
        foco = val;
    }
    
};


#endif //P3_GEOMETRYRGBFIGURES_HPP
