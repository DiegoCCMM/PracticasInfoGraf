//
// Created by diego on 11/11/20.
//

#include "Camera.hpp"
#include <cmath>

#ifndef P3_GEOMETRYRGBFIGURES_HPP
#define P3_GEOMETRYRGBFIGURES_HPP

struct RGB{
    double r, g, b;
    RGB():r(0.0), g(0.0), b(0.0){}
    RGB(double initialVal) : r(initialVal), g(initialVal), b(initialVal) {}
    RGB(double r, double g, double b) : r(r), g(g), b(b) {}

    RGB operator*(double i) const{
        return(RGB(r*i, g*i, b*i));
    }
    
    RGB operator/(double i) const{
        return(RGB(r/i, g/i, b/i));
    }

    bool operator==(const double num) const{
        return r==num && g==num && b==num;
    }

    RGB operator*(RGB x) const{
        return RGB(r*x.r, g*x.g, b*x.b);
    }

    RGB operator+(RGB x){
        return RGB(r+x.r, g+x.g, b+x.b);
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

    virtual double interseccion (const Rayo &ray) const = 0;
    virtual Vector getNormal(Punto inters) = 0;
    virtual double getFacingRatio(Rayo ray) = 0;
    
    Matriz ejeCoord (Rayo ray, Punto inters) {
        
        Vector k = this->getNormal(inters); // normal

        Vector j = (k ->* ray.getDir().normalizar()).normalizar();
        Vector i = (j ->* k).normalizar();

        Matriz resul(i, j, k);

        return resul;
    }

    void esDifuso(){
        kd = getMaxKd();
        kdTuple = RGB(red/255.0, green/255.0, blue/255.0);
    }

    void esDielectrico(){
        red = 0, green = 0, blue = 0;
        ks = 0.3;
        kt = 0.7;
    }
    
    void esRefractario(){
        kt = 1.0;
        ks = 0;
        red = 0, green = 0, blue = 0;
    }

    void esEspecular(){
        kd = getMaxKd();
        kdTuple = RGB(red/255.0, green/255.0, blue/255.0);
        ks = 1-kd;
        
    }
    void esEspejo(){
        red = 0, green = 0, blue = 0;
        ks = 1.0;
        kt = 0;
    }

    RGB getKd() const {
        return kdTuple;
    }

    double getMaxKd() const{
        return (red + green + blue) == 0 ? 0 :
                max(max(red, green), blue)/(double)(red + green + blue);
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

    RGB getRGB() const {
        return RGB(red/255.0, green/255.0, blue/255.0);
    }
};


#endif //P3_GEOMETRYRGBFIGURES_HPP
