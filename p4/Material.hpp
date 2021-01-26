//
// Created by diego on 23/1/21.
//

#include <random>
#include <cmath>
#include "Camera.hpp"
#include "geometryRGBFigures.hpp"

#ifndef P4_MATERIAL_HPP
#define P4_MATERIAL_HPP

class Material{

    double kd, ks, kt;

    void rayosSecundarios(Rayo rayo, geometryRGBFigures figure){
        //TODO fr(x, wi, w0) = kd/pi + ks(x, w0)(delta wr(wi) / n*wi) + kt(x,w0)(delta wt(wi)/n*wi)
        //TODO delta wr = 2n(n*wi) - wi
        //TODO delta wt = arcsin((n0 * sin(w0)) / n1)
        //TODO https://es.wikipedia.org/wiki/%C3%8Dndice_de_refracci%C3%B3n
        //TODO vidrio 1,45 aire 1
    }

    Vector muestreoCoseno(Rayo rayo, geometryRGBFigures figure){
        srand(NULL);

        int Einclination, Eazimuth;

        Einclination = rand();
        Eazimuth = rand();

        double inclinationi = acos(sqrt(1-Einclination));
        double azimuthi = 2*M_PI*Eazimuth;

        Matriz angulo(3,1);
        angulo.setNum(0,0, sin(inclinationi)*cos(azimuthi));
        angulo.setNum(1,0, sin(inclinationi)*sin(azimuthi));
        angulo.setNum(2,0, cos(inclinationi));

        Matriz T = figure.ejeCoord(rayo);

        Matriz matriz_wi = T*angulo;
        Vector wi = matriz_wi.vector();

        return wi;
    }

};

#endif //P4_MATERIAL_HPP
