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

void ruletaRusa(geometryRGBFigures* figure, RGB& kdColours ,double& kd, double& ks, double& kt, double &prAbs){

    // Num aleatorio para la ruleta rusa
    double e = ((double) rand() / (RAND_MAX)); // Probabilidad no evento
    // kdColours = figure->getKd();
    kd = figure->getMaxKd();
    ks = figure->getKs();
    kt = figure->getKt();

    // Normalizar para mejorar la probabilidad de no evento
    double pd = ((1.0-prAbs)*kd) / (kd+ks+kt);
    double ps = ((1.0-prAbs)*ks) / (kd+ks+kt);
    double pt = ((1.0-prAbs)*kt) / (kd+ks+kt);

    if(ks == 0.0 && kt == 0.0){//difuso
        if( e <= pd) {
            kdColours = kdColours/pd;
            //kdColours = kdColours;
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

Vector muestreoCoseno(Rayo rayo, geometryRGBFigures* figure, Punto inters) {
    double Einclination, Eazimuth;

    // TODO revisar que esten entre 0 y 1
    Einclination = ((double) rand() / (RAND_MAX));
    Eazimuth = ((double) rand() / (RAND_MAX));

    if(Einclination < 0 || Einclination > 1  || Eazimuth < 0 || Eazimuth > 1 ){

        cout<<"numeros aleatorios inccorrects"<<endl;
    }
    double inclinationi = acos(sqrt(1.0 - Einclination));
    double azimuthi = 2.0 * M_PI * Eazimuth;

    // auto primFila = sin(inclinationi) * cos(azimuthi);
    Matriz angulo(3, 1);
    angulo.setNum(0, 0, sin(inclinationi) * cos(azimuthi));
    angulo.setNum(1, 0, sin(inclinationi) * sin(azimuthi));
    angulo.setNum(2, 0, cos(inclinationi));

    // Al usar solo direcciones se pueden omitir el punto origen y la
    // ultima fila de la matriz de transformacion, porque no se pueden desplazar
    Matriz T = figure->ejeCoord(rayo, inters);

    Matriz matriz_wi = T * angulo;
    Vector wi = matriz_wi.vector();
    wi = wi.normalizar();

    return wi;
}

void nextEstimation(Rayo &rayo, list<Punto> focos, 
                    list<geometryRGBFigures*> figuras, bool& puntual) {
    // Los focos de luz puntuales tendrán la misma probabilidad
    int max = focos.size();
    if(max > 0){
        int e =  1 + rand()%max;

        list<Punto>::iterator foco = focos.begin();
        for(int i=1; i<e; i++){
            foco++;
        }

        Punto origen = rayo.getOrigen();
        Rayo r = Rayo(origen, *foco-origen);

        // Comprobar si el rayo de sombra hasta la luz puntal 'foco' intersecta con
        // algún otro objeto
        auto it = figuras.begin();
        bool colisiona = false;
        while(it != figuras.end()){
            double res = (*it)->interseccion(r);

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
    else {
        puntual = false;
    }
}


void reboteCamino(Rayo &rayo, geometryRGBFigures *figure, list<Punto> focos,
                  list<geometryRGBFigures*> figuras, double& rmax, double& gmax,
                  double& bmax, bool& puntual) {

    double kd, ks, kt, prAbs = rayo.getAbsorcion();
    RGB tupleKd = figure->getKd();

    ruletaRusa(figure, tupleKd, kd, ks, kt, prAbs);

    
    if (prAbs==1.0) {
        rayo.setAbsorcion(1.0);
    } else{
        // fr(x, wi, w0) = kd/pi + ks(x, w0)(delta wr(wi) / n*wi) + kt(x,w0)(delta wt(wi)/n*wi)
        // delta wr = 2n(n*wi) - wi
        // delta wt = arcsin((n0 * sin(w0)) / n1)
        // https://es.wikipedia.org/wiki/%C3%8Dndice_de_refracci%C3%B3n
        // vidrio 1,45 aire 1

        // Punto origen coord globales del rayo rebote
        // double distancia = figure->interseccion(rayo);
        // Vector origen_a_inter = rayo.getOrigen().sum(rayo.getDir().mul(distancia));
        // Punto inters = rayo.getOrigen()+origen_a_inter;
        Punto o = rayo.getOrigen();
        double t = figure->interseccion(rayo);
        if(t>=0){
            Vector d = rayo.getDir();
            Punto p = o + d.mul(t);

            Vector wi = muestreoCoseno(rayo, figure, p);
            Vector n = figure->getNormal(p); // normal de la fig

            if(ks != 0) { // especular - reflection

                // wi.normalizar();
                wi = (n.mul(2.0)).mul(n*wi) - wi; // diapositiva 16 - pathtracing
                wi = wi.normalizar();
                // rmax = ks/(n*wi);
                // gmax = ks/(n*wi);
                // bmax = ks/(n*wi);
                rmax = ks;
                gmax = ks;
                bmax = ks;
            }
            else if(kt != 0) { // dielectrico - refraction
                // double aire = 1.0, vidrio = 1.45; // Medios
                // Vector aux = rayo.getDir().sinV().mul(aire).div(vidrio); // TODO: revisar
                // wi = aux.asinV();
                wi = wi - (n.mul(2.0)).mul(wi*n);
                wi = wi.normalizar();
                // rmax = kt/(n*wi);
                // gmax = kt/(n*wi);
                // bmax = kt/(n*wi);
                rmax = kt;
                gmax = kt;
                bmax = kt;
            }else{  //difuso
                // if(figure->soyFoco()) {
                //     //cout << "he colisionado con la esfera difusa" << endl;
                //     rmax = tupleKd.r;
                //     gmax = tupleKd.g;
                //     bmax = tupleKd.b;
                // }
                rmax = tupleKd.r;
                gmax = tupleKd.g;
                bmax = tupleKd.b;

                if(!figure->soyFoco()) {
                    rmax *= abs(n*wi);
                    gmax *= abs(n*wi);
                    bmax *= abs(n*wi);
                }
                // else {
                //     rmax = tupleKd.r*abs(figure->getNormal(p)*wi);
                //     gmax = tupleKd.g*abs(figure->getNormal(p)*wi);
                //     bmax = tupleKd.b*abs(figure->getNormal(p)*wi);

                //     // rmax = tupleKd.r*abs(figure->getNormal(p)*wi)
                //     //             / (((1.0-prAbs)*kd) / (kd+ks+kt));
                //     // gmax = tupleKd.g*abs(figure->getNormal(p)*wi)
                //     //             / (((1.0-prAbs)*kd) / (kd+ks+kt));
                //     // bmax = tupleKd.b*abs(figure->getNormal(p)*wi)
                //     //             / (((1.0-prAbs)*kd) / (kd+ks+kt));
                // }
            }

            // wi.normalizar();

            // if(!figure->soyFoco()) {
                    // rmax *= abs(n*wi);
                    // gmax *= abs(n*wi);
                    // bmax *= abs(n*wi);
            // }

            rayo = Rayo(p, wi);
            rayo.setAbsorcion(prAbs+0.05);
        } else { // para que sirve este if - else?
            rayo = Rayo();
            rayo.setAbsorcion(1.0);
        }
        //nextEstimation(rayo, focos, figuras, puntual);
        //if(puntual){ // En caso de ser una luz puntual se divide por dist^2
            // SE ASUME QUE LAS LUCES SON DIFUSAS
        //    rmax /= pow(rayo.getDir().module(),2);
        //    gmax /= pow(rayo.getDir().module(),2);
        //    bmax /= pow(rayo.getDir().module(),2);
        //}
    }



}



#endif //P4_MATERIAL_HPP
