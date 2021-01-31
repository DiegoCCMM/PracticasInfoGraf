//
// Created by diego on 23/1/21.
//

#include <random>
#include <cmath>
#include "Camera.hpp"
#include "geometryRGBFigures.hpp"

#ifndef P4_MATERIAL_HPP
#define P4_MATERIAL_HPP

void ruletaRusa(geometryRGBFigures figure, double& kd, double& ks, double& kt, double &prAbs){
    srand(NULL);

    double e = ((double) rand() / (RAND_MAX));

    kd = figure.getKd();
    ks = figure.getKs();
    kt = figure.getKt();

    // Normalizar para mejorar la probabilidad de no evento
    double pd = ((1-prAbs)*kd) / (kd+ks+kt);
    double ps = ((1-prAbs)*ks) / (kd+ks+kt);
    double pt = ((1-prAbs)*kt) / (kd+ks+kt);

    if(ks == 0.0 && kt == 0.0){//difuso
        if( e <= pd) {
            kd = kd/pd;
        }else{
            prAbs = 1.0;
            kd = 0.0;
        }
    }else if(kt == 0.0){//plastico
        if( e <= pd ){//difuso
            kd = kd/pd;
            ks = 0.0;
        }else if( e > pd && e <= pd+ps ){  //especular
            ks = ks/pd;
            kd = 0.0;
        }else{ //absorbido
            prAbs = 1.0;
            ks = 0.0;
            kd = 0.0;
        }
    }else if(kd == 0.0){//dielectrico
        if( e <= pt ){//difuso
            kt = kt/pt;
            ks = 0.0;
        }else if( e > pt && e <= pt+ps){  //especular
            ks = ks/pd;
            kt = 0.0;
        }else{  //absorbido
            prAbs = 1.0;
            ks = 0.0;
            kt = 0.0;
        }
    }
}

void reboteCamino(Rayo &rayo, geometryRGBFigures figure) {
    double kd, ks, kt, prAbs = rayo.getAbsorcion();
    ruletaRusa(figure, kd, ks, kt, prAbs);

    //TODO fr(x, wi, w0) = kd/pi + ks(x, w0)(delta wr(wi) / n*wi) + kt(x,w0)(delta wt(wi)/n*wi)
    //TODO delta wr = 2n(n*wi) - wi
    //TODO delta wt = arcsin((n0 * sin(w0)) / n1)
    //TODO https://es.wikipedia.org/wiki/%C3%8Dndice_de_refracci%C3%B3n
    //TODO vidrio 1,45 aire 1

    // todo calcular el punto origen del rayo rebote
    rayo = Rayo(origen, );
    
    if (figure.soyFoco() || prAbs==1.0) {
        rayo.setAbsorcion(1.0);
    } else{
        rayo.setAbsorcion(prAbs+0.05);
    }

}

Vector muestreoCoseno(Rayo rayo, geometryRGBFigures figure) {
    srand(NULL);

    double Einclination, Eazimuth;

    Einclination = ((double) rand() / (RAND_MAX));
    Eazimuth = ((double) rand() / (RAND_MAX));

    double inclinationi = acos(sqrt(1 - Einclination));
    double azimuthi = 2 * M_PI * Eazimuth;

    Matriz angulo(3, 1);
    angulo.setNum(0, 0, sin(inclinationi) * cos(azimuthi));
    angulo.setNum(1, 0, sin(inclinationi) * sin(azimuthi));
    angulo.setNum(2, 0, cos(inclinationi));

    Matriz T = figure.ejeCoord(rayo);

    Matriz matriz_wi = T * angulo;
    Vector wi = matriz_wi.vector();

    return wi;
}


#endif //P4_MATERIAL_HPP
