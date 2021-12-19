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

enum Evento {
    DIFUSO, ESPECULAR, DIELECTRICO, NOEVENTO
};

Evento ruletaRusa(geometryRGBFigures* figure, const double &probabilidad_absorcion){

    // Num aleatorio para la ruleta rusa
    double e = ((double) rand() / (RAND_MAX)); // Probabilidad no evento
    double kd = figure->getMaxKd(),
           ks = figure->getKs(),
           kt = figure->getKt();

    // Normalizar para mejorar la probabilidad de no evento
    double pd = ((1.0-probabilidad_absorcion)*kd) / (kd+ks+kt);
    double ps = ((1.0-probabilidad_absorcion)*ks) / (kd+ks+kt);
    double pt = ((1.0-probabilidad_absorcion)*kt) / (kd+ks+kt);

    if(ks == 0.0 && kt == 0.0){//difuso KD y no tiene KS ni KT
        if( e <= pd) {
            // kdColours = kdColours/pd;
            return DIFUSO;
        }else{
            // prAbs = 1.0;
            // kd = 0.0;
            return NOEVENTO;
        }
    }else if(kt == 0.0){//plastico  KD y KS y no tiene KT
        if( e <= pd ){//difuso
            // kdColours = kdColours/pd;
            // ks = 0.0;
            return DIFUSO;
        }else if( e > pd && e <= pd+ps ){  //especular
            // ks = ks/ps;
            // kd = 0.0;
            return ESPECULAR;
        }else{ //absorbido
            // prAbs = 1.0;
            // ks = 0.0;
            // kd = 0.0;
            return NOEVENTO;
        }
    }else if(kd == 0.0){//dielectrico   KS Y KT y no tiene KD
        if( e <= pt ){//transparente
            // kt = kt/pt;
            // ks = 0.0;
            return DIELECTRICO;
        }else if( e > pt && e <= pt+ps){  //especular
            // ks = ks/ps;
            // kt = 0.0;
            return ESPECULAR;
        }else{  //absorbido
            // prAbs = 1.0;
            // ks = 0.0;
            // kt = 0.0;
            return NOEVENTO;
        }
    }
}

Vector muestreoCoseno(const Rayo &rayo, geometryRGBFigures* figure, Punto inters) {
    double Einclination, Eazimuth;

    Einclination = ((double) rand() / (RAND_MAX));
    Eazimuth = ((double) rand() / (RAND_MAX));

    if(Einclination < 0 || Einclination > 1  || Eazimuth < 0 || Eazimuth > 1 ){

        cout<<"numeros aleatorios inccorrects"<<endl;
    }
    double inclinationi = acos(sqrt(1.0 - Einclination));
    double azimuthi = 2.0 * M_PI * Eazimuth;

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

Punto getPuntoInters(const Rayo &rayo, geometryRGBFigures* &figure) {
    double t = figure->interseccion(rayo); // distancia

    return rayo.getOrigen() + rayo.getDir().mul(t); // puntoInterseccion
}

RGB colorBDRF(const Evento &evento, const Rayo &rayoEntrante, geometryRGBFigures* figure,
        Punto punto_inters) {

    Vector normalFigura = figure->getNormal(punto_inters);
    
    double cos = abs(normalFigura * rayoEntrante.getDir());

    double kd = figure->getMaxKd(),
           ks = figure->getKs(),
           kt = figure->getKt();

    double pd = ((1.0-rayoEntrante.getAbsorcion())*kd) / (kd+ks+kt);
    double ps = ((1.0-rayoEntrante.getAbsorcion())*ks) / (kd+ks+kt);
    double pt = ((1.0-rayoEntrante.getAbsorcion())*kt) / (kd+ks+kt);

    switch (evento) {
        case DIFUSO:
            return (kd/pd) * cos / M_PI;
        case ESPECULAR:
            return ks/ps;
        case DIELECTRICO:
            return kt/pt;
        case NOEVENTO:
            return RGB(0.0);
    }
}

// Indica si un rayo ha colisionado con algún objeto o no, y en caso de hacerlo
// se guarda la figura
bool hayColision(const list<geometryRGBFigures*> &figuras, const Rayo &rayoEntrante, 
        geometryRGBFigures* &figure){

    int max = INT_MAX;
    bool colisiona = false;
    for(auto it = figuras.begin(); it != figuras.end(); it++){
        double res = (*it)->interseccion(rayoEntrante);
        // double res = 0;

        if(res >= 0 && res < max){
            max = res;
            figure = (*it);
            colisiona = true;
        }
    }
    
    return colisiona;
}

RGB colorLuzDirecta(const Rayo &rayoEntrante, const list<FocoPuntual> &focos,  
        const list<geometryRGBFigures*> &figuras, const Evento &evento, const RGB &Throughput) {

    RGB Radiance(0.0);
    if(focos.size() > 0){
        Punto origen = rayoEntrante.getOrigen();
        for(auto foco = focos.begin(); foco != focos.end(); foco++){
        
            Punto posicion_foco = foco->getPosition();
            Rayo rayoSombra = Rayo(origen, (posicion_foco-origen).normalizar());

            // Comprobar si el rayo de sombra hasta la luz puntal 'foco' intersecta con
            // algún otro objeto antes de la luz puntual
            geometryRGBFigures* figura_intersectada;
            bool colisiona = hayColision(figuras, rayoSombra, figura_intersectada);

            if(!colisiona) {
                // int intensity = 1;
                Punto punto_inters = getPuntoInters(rayoSombra, figura_intersectada);
                Radiance = Radiance + foco->getKd() * colorBDRF(evento, rayoSombra, figura_intersectada, punto_inters) * Throughput / pow(rayoSombra.getDir().module(),2);
            }
        }
    }
    // if(Radiance.r > 1.0) Radiance.r = 1.0;
    // if(Radiance.g > 1.0) Radiance.g = 1.0;
    // if(Radiance.b > 1.0) Radiance.b = 1.0;

    return Radiance;
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

Vector nuevaDireccion(const Rayo &rayoEntrante, geometryRGBFigures* &figure, 
        const Evento &evento){

    Vector wi;
    Punto inters = getPuntoInters(rayoEntrante, figure);
    Vector n = figure->getNormal(inters); // normal de la fig

    if(evento == ESPECULAR) { // especular - reflection
        
        wi = rayoEntrante.getDir().normalizar() - (n.mul(2.0)).mul(n*rayoEntrante.getDir().normalizar());
        wi = wi.normalizar();
    }
    else if(evento == DIELECTRICO) { // dielectrico - refraction

        double aire = 1.0, vidrio = 1.45; // Medios
        double cosenoAnguloIncidencia = clamp(-1, 1, rayoEntrante.getDir().normalizar()*n);

        Vector N = n; 
        
        if (cosenoAnguloIncidencia < 0 ) 
        { cosenoAnguloIncidencia = -cosenoAnguloIncidencia; } else { std::swap(aire, vidrio); N = invert(n); } 

        double relacionMedios = aire / vidrio; 
        double k = 1 - relacionMedios * relacionMedios * (1 - cosenoAnguloIncidencia * cosenoAnguloIncidencia); 
        k < 0 ? wi = Vector(0,0,0) : wi = (rayoEntrante.getDir().normalizar().mul(relacionMedios) + 
        N.mul(relacionMedios * cosenoAnguloIncidencia - (double)sqrtf(k)));
    }
    else if(evento == DIFUSO) {  //difuso

        wi = muestreoCoseno(rayoEntrante, figure, inters);        
    }

    return wi;
}

RGB colorCamino(const list<FocoPuntual> &focos, const list<geometryRGBFigures*> &figuras, 
        Rayo rayoEntrante) {

    RGB Throughput(1.0), Radiance(0.0);

    int i;
    for(i = 1 ;; i++){
        // Se verifica si el rayoEntrante intersecta con algún objeto
        geometryRGBFigures* figura_intersectada;
        bool colisiona = hayColision(figuras, rayoEntrante, figura_intersectada);

        if (!colisiona) {
            // Si no colisiona con ningún objeto, el rayo se pierde, pero puede
            // haber radiancia, por lo que no se devuelve el valor directamente
            Throughput = RGB(0.0);
            break;
        }
        else{
            if(figura_intersectada->soyFoco()){
                Throughput = Throughput * figura_intersectada->getKd();
                break;
            }
            else{
                // Objeto que no es emisor
                // Se calcula el next event mediante ruleta rusa
                // RGB tupleKd = figura_intersectada->getKd();

                Evento evento = ruletaRusa(figura_intersectada, rayoEntrante.getAbsorcion());
                if(evento == NOEVENTO) {
                    // El rayo muere
                    Throughput = RGB(0.0);
                    break;
                }
                else {
                    // Calcular la radiancia - luces directas
                    Radiance = Radiance + colorLuzDirecta(rayoEntrante, focos, figuras, evento, Throughput);

                    // Se crea el rayo del rebote
                    Vector nuevaDir = nuevaDireccion(rayoEntrante, figura_intersectada, evento);
                    Rayo rayoSaliente = Rayo(getPuntoInters(rayoEntrante, figura_intersectada), nuevaDir);
                    rayoSaliente.setAbsorcion(rayoEntrante.getAbsorcion() + 0.05);

                    
                    Punto punto_inters = getPuntoInters(rayoEntrante, figura_intersectada);
                    Throughput = Throughput * colorBDRF(evento, rayoSaliente, figura_intersectada, punto_inters);
                    rayoEntrante = rayoSaliente;

                }
            }
        }
    }

    return Throughput + Radiance / i;
}


#endif //P4_MATERIAL_HPP
