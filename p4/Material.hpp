//
// Created by diego on 23/1/21.
//

#include <random>
#include <cmath>
#include "Camera.hpp"
#include "geometryRGBFigures.hpp"
#include "FocoPuntual.hpp"
#include <list>
#include <limits.h>

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

    if(ks == 0.0 && kt == 0.0){//difuso KD y no tiene KS ni KT
        if( e <= pd) {
            kdColours = kdColours/pd;
            //kdColours = kdColours;
        }else{
            prAbs = 1.0;
            kd = 0.0;
        }
    }else if(kt == 0.0){//plastico  KD y KS y no tiene KT
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
    }else if(kd == 0.0){//dielectrico   KS Y KT y no tiene KD
        if( e <= pt ){//transparente
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

void nextEstimation(Rayo rayo, list<FocoPuntual> focos, const geometryRGBFigures *figure,
                    list<geometryRGBFigures*> figuras, 
                    RGB& Radiance, RGB Throughput) {
    // Los focos de luz puntuales tendrán la misma probabilidad
    if(focos.size() > 0){
        Punto origen = rayo.getOrigen();
        for(auto foco = focos.begin(); foco != focos.end(); foco++){
        
            Punto posicion_foco = foco->getPosition();
            Rayo r = Rayo(origen, (posicion_foco-origen).normalizar());

            // Comprobar si el rayo de sombra hasta la luz puntal 'foco' intersecta con
            // algún otro objeto
            bool colisiona = false;

            // for(auto it = figuras.begin(); it != figuras.end(); it++){
            for(auto it = figuras.begin(); it != figuras.end() && figure != *it; it++){
                double res = (*it)->interseccion(r);
                // Solo nos importa si intersecta con alguna fig por el camino
                if(res >= 0){ 
                    colisiona = true;
                    break;
                }
            }

            if(!colisiona) {     
                Radiance = Radiance * Throughput * foco->getRed()/255.0 / pow(r.getDir().module(),2);
                // Radiance.r += rThr * foco->getRed()/255.0 / pow(r.getDir().module(),2);
                // Radiance.g += gThr * foco->getGreen()/255.0 / pow(r.getDir().module(),2);
                // Radiance.b += bThr * foco->getBlue()/255.0 / pow(r.getDir().module(),2);
                // const int intensity = 1;
                // Radiance.r += Rebote.r * (foco->getRed()/255.0) * intensity * (*figure).getFacingRatio(r) / pow(r.getDir().module(),2) * M_PI;
                // Radiance.g += Rebote.g * (foco->getGreen()/255.0) * intensity * (*figure).getFacingRatio(r) / pow(r.getDir().module(),2) * M_PI;
                // Radiance.b += Rebote.b * (foco->getBlue()/255.0) * intensity * (*figure).getFacingRatio(r) / pow(r.getDir().module(),2) * M_PI;
                return;
                // Se sale porque ya se ha encontrado una luz puntual sin
                // sin intersectar con ningun otro objeto
            }
        }
    }
}

//clamp the value between min and max
double clamp ( double min, double max, double val){
    if(val>max)
     return max;
    else if(val < min)
     return min;
    else
     return val;
}
//invert the direction of the vector
Vector invert(Vector invertable){
    return(Vector(-invertable.x, -invertable.y, -invertable.z));
}

void reboteCamino(Rayo &rayo, geometryRGBFigures *figure, list<FocoPuntual> focos,
                  list<geometryRGBFigures*> figuras, RGB &Radiance, RGB &Throughput) {

    double kd, ks, kt, prAbs = rayo.getAbsorcion();
    RGB tupleKd = figure->getKd();

    ruletaRusa(figure, tupleKd, kd, ks, kt, prAbs);

    if (prAbs==1.0) {
        rayo.setAbsorcion(1.0);
        // rayo.setLuzPuntual(false);
        // si se absorbe no hay next event estimation
    } else{
        RGB Rebote;
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

        Vector d = rayo.getDir();
        Punto p = o + d.mul(t);

        Vector wi = muestreoCoseno(rayo, figure, p);
        Vector n = figure->getNormal(p); // normal de la fig

        if(ks != 0) { // especular - reflection

            // wi.normalizar();
            // wi = (n.mul(2.0)).mul(n*wi) - wi; // diapositiva 16 - pathtracing
            // wi = (n.mul(2.0)).mul(n*rayo.getDir()) - rayo.getDir(); // diapositiva 16 - pathtracing
            wi = rayo.getDir().normalizar() - (n.mul(2.0)).mul(n*rayo.getDir().normalizar());
            wi = wi.normalizar();
            // rmax = ks/(n*wi);
            // gmax = ks/(n*wi);
            // bmax = ks/(n*wi);
            Rebote.r = ks;
            Rebote.g = ks;
            Rebote.b = ks;

            Rebote.r *= abs(n*wi);
            Rebote.g *= abs(n*wi);
            Rebote.b *= abs(n*wi);
        }
        else if(kt != 0) { // dielectrico - refraction
            double aire = 1.0, vidrio = 1.45; // Medios
            double cosenoAnguloIncidencia = clamp(-1, 1, rayo.getDir().normalizar()*n);

            Vector N = n; 
            
            if (cosenoAnguloIncidencia < 0 ) 
            { cosenoAnguloIncidencia = -cosenoAnguloIncidencia; } else { std::swap(aire, vidrio); N = invert(n); } 

            double relacionMedios = aire / vidrio; 
            double k = 1 - relacionMedios * relacionMedios * (1 - cosenoAnguloIncidencia * cosenoAnguloIncidencia); 
            k < 0 ? wi = Vector(0,0,0) : wi = (rayo.getDir().normalizar().mul(relacionMedios) + 
            N.mul(relacionMedios * cosenoAnguloIncidencia - (double)sqrtf(k))); 
            /*Vector aux = rayo.getDir().normalizar().sinV().mul(aire).div(vidrio); 
            wi = aux.asinV().normalizar();*/
            /*wi = wi - (n.mul(2.0)).mul(wi*n);
            wi = wi.normalizar();*/
            // rmax = kt/(n*wi);
            // gmax = kt/(n*wi);
            // bmax = kt/(n*wi);
            Rebote.r = kt;
            Rebote.g = kt;
            Rebote.b = kt;
        }else{  //difuso
            // if(figure->soyFoco()) {
            //     //cout << "he colisionado con la esfera difusa" << endl;
            //     rmax = tupleKd.r;
            //     gmax = tupleKd.g;
            //     bmax = tupleKd.b;
            // }
            Rebote.r = tupleKd.r;
            Rebote.g = tupleKd.g;
            Rebote.b = tupleKd.b;

            // if(!figure->soyFoco()) {
                Rebote.r *= abs(n*wi);
                Rebote.g *= abs(n*wi);
                Rebote.b *= abs(n*wi);
            // }
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
        // ahora mismo nextEstimation se aplica también a las areas de luz
        nextEstimation(rayo, focos, figure, figuras, Radiance, Throughput);
        
        Throughput = Throughput * Rebote;
    }
}


#endif //P4_MATERIAL_HPP
