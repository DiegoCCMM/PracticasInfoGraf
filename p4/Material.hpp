//
// Created by diego on 23/1/21.
//

#include <random>
#include <cmath>
#include "Camera.hpp"
#include "geometryRGBFigures.hpp"
#include <list>

#ifndef P4_MATERIAL_HPP
#define P4_MATERIAL_HPP

void ruletaRusa(geometryRGBFigures figure, RGB& kdColours ,double& kd, double& ks, double& kt, double &prAbs){
    srand(NULL);

    double e = ((double) rand() / (RAND_MAX));
    kdColours = figure.getKd();
    kd = figure.getMaxKd();
    ks = figure.getKs();
    kt = figure.getKt();

    // Normalizar para mejorar la probabilidad de no evento
    double pd = ((1-prAbs)*kd) / (kd+ks+kt);
    double ps = ((1-prAbs)*ks) / (kd+ks+kt);
    double pt = ((1-prAbs)*kt) / (kd+ks+kt);

    if(ks == 0.0 && kt == 0.0){//difuso
        if( e <= pd) {
            kdColours = kdColours/pd;
        }else{
            prAbs = 1.0;
            kd = 0.0;
        }
    }else if(kt == 0.0){//plastico
        if( e <= pd ){//difuso
            kdColours = kdColours/pd;
            ks = 0.0;
        }else if( e > pd && e <= pd+ps ){  //especular
            ks = ks/ps;
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
            ks = ks/ps;
            kt = 0.0;
        }else{  //absorbido
            prAbs = 1.0;
            ks = 0.0;
            kt = 0.0;
        }
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

void nextEstimation(Rayo &rayo, list<Punto> focos, 
                    list<geometryRGBFigures> figuras, bool& puntual) {
    // Los focos de luz puntuales tendrán la misma probabilidad
    int max = focos.size();
    int e =  1 + rand()%max;

    list<Punto>::iterator foco = focos.begin();
    for(int i=1; i<e; i++){
        foco++;
    }

    Punto origen = rayo.getOrigen();
    Rayo r = Rayo(origen, *foco-origen);

    // Comprobar si el rayo de sombra hasta la luz puntal 'foco' intersecta con
    // algún otro objeto
    list<geometryRGBFigures>::iterator it = figuras.begin();
    bool colisiona = false;
    while(it != figuras.end()){
        double res = (*it).interseccion(r);

        if(res > 0 && res < max){
            max = res;
            colisiona = true;
        }
        it++;
    }

    if(!colisiona) {
        rayo = r;
        puntual = true;
    } else {
        puntual = false;
    }
    
}


void reboteCamino(Rayo &rayo, geometryRGBFigures figure, list<Punto> focos,
                    list<geometryRGBFigures> figuras, double& rmax, double& gmax, 
                    double& bmax, bool& puntual) {

    double kd, ks, kt, prAbs = rayo.getAbsorcion();
    RGB tupleKd = figure.getKd();

    ruletaRusa(figure, tupleKd, kd, ks, kt, prAbs);

    
    if (prAbs==1.0) {
        rayo.setAbsorcion(1.0);
    } else{
        rayo.setAbsorcion(prAbs+0.05);
        // fr(x, wi, w0) = kd/pi + ks(x, w0)(delta wr(wi) / n*wi) + kt(x,w0)(delta wt(wi)/n*wi)
        // delta wr = 2n(n*wi) - wi
        // delta wt = arcsin((n0 * sin(w0)) / n1)
        // https://es.wikipedia.org/wiki/%C3%8Dndice_de_refracci%C3%B3n
        // vidrio 1,45 aire 1
        // Punto origen coord globales del rayo rebote
        **********Punto origen = rayo.getOrigen()+rayo.getDir(); // MAL

        Vector wi = muestreoCoseno(rayo, figure);
        if(ks != 0) { // especular

            // TODO
            // 1. calcular plano a partir del punto origen (interseccion)
            // 2. calcular normal al plano
            ********Vector n;

            wi = n.mul(2.0) ->* n ->* wi - wi;
            rmax = ks;
            gmax = ks;
            bmax = ks;
        }
        else if(kt != 0) { // dielectrico
            double aire = 1.0, vidrio = 1.45; // Medios
            Vector aux = rayo.getDir().sin().mul(aire).div(vidrio);
            wi = aux.asin();
            rmax = kt;
            gmax = kt;
            bmax = kt;
        }else{  //difuso
            rmax = tupleKd.r;
            gmax = tupleKd.g;
            bmax = tupleKd.b;

            if(puntual){ // En caso de ser una luz puntual se divide por dist^2
                // SE ASUME QUE LAS LUCES SON DIFUSAS
                rmax /= rayo.getDir().module();
                gmax /= rayo.getDir().module();
                bmax /= rayo.getDir().module();
            }
        }

        rayo = Rayo(origen, wi);
        nextEstimation(rayo, focos, figuras, puntual);
    }



}



#endif //P4_MATERIAL_HPP
