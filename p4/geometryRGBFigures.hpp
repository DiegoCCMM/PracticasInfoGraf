//
// Created by diego on 11/11/20.
//

#include "Camera.hpp"
#include <cmath>

#ifndef P3_GEOMETRYRGBFIGURES_HPP
#define P3_GEOMETRYRGBFIGURES_HPP

struct RGB{
    double r, g, b;

    RGB(double r, double g, double b) : r(r), g(g), b(b) {}

    RGB operator*(double i) const{
        return(RGB(r*i, g*i, b*i));
    }
    RGB operator/(double i) const{
        return(RGB(r/i, g/i, b/i));
    }

};


class geometryRGBFigures {

public:

    int red, green, blue;

    RGB kdTuple = RGB(0,0,0);
    double kd=0.0, ks=0.0, kt=0.0;

    bool foco = false;

    int getRed()   const { return red;   }
    int getGreen() const { return green; }
    int getBlue()  const { return blue;  }

    virtual double interseccion (Rayo ray) {}
    virtual Vector getNormal(Punto inters) {}
    
    Matriz ejeCoord (Rayo ray, Punto inters) {
        // Vector i = ray.getDir().normalizar()->*this->getNormal(inters).normalizar();
        // // i.normalizar();
        // Vector j = i->*this->getNormal(inters).normalizar();
        // // j.normalizar();

        // Matriz resul(i, j, this->getNormal(inters).normalizar());

        // return resul;

        // Vector i = ray.getDir()->*this->getNormal(inters);
        // i.normalizar();
        // Vector j = i->*this->getNormal(inters).normalizar();

        // Matriz resul(i, j, this->getNormal(inters).normalizar());
        Vector k = this->getNormal(inters); // normal

        Vector j = (k ->* ray.getDir().normalizar()).normalizar();
        if((k ->* ray.getDir()).module()<0.99){
            int p = 0;
        }
        // i.normalizar();
        Vector i = (j ->* k).normalizar();

        Matriz resul(i, j, k);

        return resul;
    }

    void esDifuso(){
        kd = max(max(red, green), blue)/(double)(red + green + blue);
        kdTuple = RGB(red/255.0, green/255.0, blue/255.0);
    }

    void esDielectrico(){
        ks = 0.4;
        kt = 0.6;
    }

    void esEspecular(){
        kd = max(max(red, green), blue)/(double)(red + green + blue);
        kdTuple = RGB(red/255.0, green/255.0, blue/255.0);
        ks = 1-kd;
    }

    RGB getKd() const {
        return kdTuple;
    }

    double getMaxKd() const{
        return max(max(red, green), blue)/(double)(red + green + blue);
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
