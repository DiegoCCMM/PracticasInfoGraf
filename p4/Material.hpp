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
            return DIFUSO;
        }else{
            return NOEVENTO;
        }
    }else if(kt == 0.0){//plastico  KD y KS y no tiene KT
        if( e <= pd ){//difuso
            return DIFUSO;
        }else if( e > pd && e <= pd+ps ){  //especular
            return ESPECULAR;
        }else{ //absorbido
            return NOEVENTO;
        }
    }else if(kd == 0.0){//dielectrico   KS Y KT y no tiene KD
        if( e <= pt ){//transparente
            return DIELECTRICO;
        }else if( e > pt && e <= pt+ps){  //especular
            return ESPECULAR;
        }else{  //absorbido
            return NOEVENTO;
        }
    }
    exit(1);
}

Vector muestreoCoseno(const Rayo &rayo, geometryRGBFigures* figure, Punto inters) {
    double Einclination, Eazimuth;

    Einclination = ((double) rand() / (RAND_MAX));
    Eazimuth = ((double) rand() / (RAND_MAX));

    if(Einclination < 0 || Einclination > 1  || Eazimuth < 0 || Eazimuth > 1 ){
        cout<<"numeros aleatorios inccorrectos"<<endl;
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

    //std::cout << wi.x << " " << wi.y << " " << wi.z << std::endl;

    return wi;
}

Punto getPuntoInters(const Rayo &rayo, geometryRGBFigures* &figure) {
    double dist = figure->interseccion(rayo); // distancia

    return rayo.getOrigen() + rayo.getDir().mul(dist); // puntoInterseccion
}

RGB colorBRDF(const Evento &evento, geometryRGBFigures* figure, const double &cos_wi) {

    switch (evento) {
        case DIFUSO:
            return figure->getKd() / M_PI; // colores más apagados PERO *LA FÓRMULA ES ASÍ*
            // return figure->getKd(); // colores más vivos pero puede que más ruido
        case ESPECULAR:
            return RGB(figure->getKs()/cos_wi);
        case DIELECTRICO:
            return RGB(figure->getKt()/cos_wi);
        case NOEVENTO:
            return RGB(0.0);
        default:
            exit(1);
    }
}

// Indica si un rayo ha colisionado con algún objeto o no, y en caso de hacerlo
// se guarda la figura
bool hayColision(const list<geometryRGBFigures*> &figuras, const Rayo &rayoEntrante, 
        geometryRGBFigures* &figure){

    double max = (double)INT_MAX;
    bool colisiona = false;
    for(const auto &it : figuras){
        double res = it->interseccion(rayoEntrante);

        if(res >= 0 && res < max){
            max = res;
            figure = it;
            colisiona = true;
        }
    }
    
    return colisiona;
}

RGB colorLuzDirecta(const Rayo &rayoEntrante, const list<FocoPuntual> &focos,  
        const list<geometryRGBFigures*> &figuras, const Evento &evento, 
        const RGB &Throughput, geometryRGBFigures* figura_intersectada,
        const double &cos_wi) {

    RGB Radiance(0.0);
    if(focos.size() > 0){
        Punto punto_inters = getPuntoInters(rayoEntrante, figura_intersectada);
        for(const auto &foco : focos){
        
            Punto posicion_foco = foco.getPosition();
            Vector dirSombra = (posicion_foco - punto_inters).normalizar();
            double dist = (posicion_foco - punto_inters).module(); // Distancia del foco al objeto
            Rayo rayoSombra = Rayo(punto_inters, dirSombra);

            // Comprobar si el rayo de sombra hasta la luz puntal 'foco' intersecta con
            // algún otro objeto antes de la luz puntual
            bool colisiona = false;
            for(const auto &it : figuras){
                    double res = it->interseccion(rayoSombra);

                    if(res >= 0 && res < dist){
                        colisiona = true;
                        break;
                    }
            }

            if(!colisiona) {
                // max(0,n*wi)  si es negativo quita luz
                double dotProduct = figura_intersectada->getNormal(punto_inters) * rayoSombra.getDir();
                double cos = dotProduct <= 0 ? 0 : dotProduct;
                Radiance = Radiance + Throughput * (foco.getRGB() / pow(rayoSombra.getDir().module(),2)) * colorBRDF(evento, figura_intersectada, cos_wi) * cos;
            }
        }
        return Radiance / focos.size();
    }
    return Radiance; // Hacer media?
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
    Vector normal_fig = figure->getNormal(inters); // normal de la fig

    switch (evento) {
        case ESPECULAR: { // especular - reflection
            wi = rayoEntrante.getDir() - (normal_fig.mul(2.0)).mul(normal_fig*rayoEntrante.getDir());
            break;
        }
        case DIELECTRICO: {
            double aire = 1.0, vidrio = 1.45; // Medios
            double cosenoAnguloIncidencia = clamp(-1, 1, rayoEntrante.getDir()*normal_fig);

            Vector N = normal_fig; 
            
            if (cosenoAnguloIncidencia < 0 ) 
            { cosenoAnguloIncidencia = -cosenoAnguloIncidencia; } else { std::swap(aire, vidrio); N = invert(normal_fig); } 

            double relacionMedios = aire / vidrio; 
            double k = 1 - relacionMedios * relacionMedios * (1 - cosenoAnguloIncidencia * cosenoAnguloIncidencia); 
            k < 0 ? wi = Vector(0,0,0) : wi = (rayoEntrante.getDir().mul(relacionMedios) + 
            N.mul(relacionMedios * cosenoAnguloIncidencia - (double)sqrtf(k)));
            break;
        }
        case DIFUSO:{
            wi = muestreoCoseno(rayoEntrante, figure, inters);
            break;
        }
    }

    return wi.normalizar();
}

double getPdf(Evento evento, geometryRGBFigures* figura_intersectada, double prob_absorcion){
    double kd = figura_intersectada->getMaxKd(),
           ks = figura_intersectada->getKs(),
           kt = figura_intersectada->getKt();

    switch (evento) {
        case ESPECULAR: // especular - reflection
            return ((1.0-prob_absorcion)*ks) / (kd+ks+kt); // ps
        case DIELECTRICO:
            return ((1.0-prob_absorcion)*kt) / (kd+ks+kt); // pt
        case DIFUSO:
            return ((1.0-prob_absorcion)*kd) / (kd+ks+kt); // pd
        default:
            exit(1);
    }
}

RGB colorCamino(const list<FocoPuntual> &focos, const list<geometryRGBFigures*> &figuras, 
        Rayo rayoEntrante) {

    RGB Throughput(1.0), Radiance(0.0);

    // Number of paths that go through the pixel
    while(true){
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
            else{ // Objeto que no es emisor
                 
                // Se calcula el next event mediante ruleta rusa
                Evento evento = ruletaRusa(figura_intersectada, rayoEntrante.getAbsorcion());
                if(evento == NOEVENTO) {
                    // El rayo muere
                    Throughput = RGB(0.0);
                    break;
                }
                else {
                    Punto punto_inters = getPuntoInters(rayoEntrante, figura_intersectada);
                    Vector normal_fig_inters = figura_intersectada->getNormal(punto_inters);

                    // Se crea el rayo del rebote
                    Vector nuevaDir = nuevaDireccion(rayoEntrante, figura_intersectada, evento);
                    Rayo rayoSaliente = Rayo(punto_inters, nuevaDir);
                    rayoSaliente.setAbsorcion(rayoEntrante.getAbsorcion() + 0.05);                    
                    
                    double cos_wi = abs(rayoSaliente.getDir() * normal_fig_inters);

                    // Calcular la radiancia - luces directas
                    Radiance = Radiance + colorLuzDirecta(rayoEntrante, focos, figuras, evento, Throughput, figura_intersectada, cos_wi);
                                        
                    Throughput = Throughput * colorBRDF(evento, figura_intersectada, cos_wi) * abs(cos_wi) / getPdf(evento, figura_intersectada, rayoEntrante.getAbsorcion());
                    rayoEntrante = rayoSaliente;
                }
            }
        }
    }

    return Throughput + Radiance;
}


#endif //P4_MATERIAL_HPP
