//
// Created by diego on 22/1/21.
//

//
// Created by diego on 7/11/20.
//

#include <string>
#include <fstream>
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "../p1/Matriz.hpp"
#include <list>


int main(int argc, char* argv[]){

    int pixelRes = stoi(argv[1]); // Número de rayos (?) (1048576)
    ofstream ldrfile;
    ldrfile.open(argv[2]);

    int width = 1024,
            height = 1024;


    // Escena
    Sphere sphere1 = Sphere(Punto(0,0,2200), 20.0, 34, 153, 84);    // Verde
    Sphere sphere2 = Sphere(Punto(20,20,2220), 20.0, 205, 92, 92);  // Roja

    list<Sphere> figuras;
    figuras.push_back(sphere1);
    figuras.push_back(sphere2);

    // Sistema de coordenadas de la cámara
    int front = 2000;
    Vector  x = Vector(width/2.0,0,0),
            y = Vector(0,width/2.0,0),
            z = Vector(0,0,front);


    Punto origen = Punto(0,0,0);
    //Sistemas de coordenadas en matriz para hacer el cambio de sistemas
    Matriz siscam = Matriz(x,y,z,origen);

    //plano de proyección
    double area = width*height;
    double pixelUnit = area / (double)pixelRes; // medidas de cada pixel

    //variables para el for
    Rayo r;
    double xLocal, yLocal = height/2.0 - pixelUnit/2.0, max = -1;
    int rmax, gmax, bmax;
    Vector dirLocal, dirGlobal;

    //cuantos pixeles tendrá cada lado
    int numPixAncho = width/pixelUnit;
    int numPixAlto = height/pixelUnit;

    ldrfile << "P3" << endl;
    ldrfile << "#MAX=255" << endl;
    ldrfile << numPixAncho << " " << numPixAlto << endl;
    ldrfile << 255 << endl;

    // Procedimiento: izq -> der, arriba -> abajo
    for (int j = 0; j < numPixAlto; j++) {

        xLocal = -width/2.0 + pixelUnit/2.0;

        for (int i = 0; i < numPixAncho; i++) {
            double dist = sqrt(pow(xLocal,2) + pow(yLocal,2));
            double posZ = sqrt(pow(dist,2) + pow(front,2));
            double normalized = Vector(xLocal, yLocal, posZ).module();
            dirLocal = Vector(xLocal/normalized, yLocal/normalized, 1);

            //Vector con la dirección local a la matriz de proyección
            //de tipo matriz para poder operar con la matriz de cambio de base
            Matriz local = Matriz(dirLocal, 0);

            //cambio de base, de salida tendremos la dirección del vector en coordenadas globales
            Matriz Global = siscam * local;

            r = Rayo(origen, Global.vector());

            max = 3;
            rmax = 0;
            gmax = 0;
            bmax = 0;
            list<Sphere>::iterator it = figuras.begin();
            while(it != figuras.end()){
                double res = (*it).interseccion(r);

                if(res > 0 && res < max){
                    max = res;
                    rmax = (*it).getRed();
                    gmax = (*it).getGreen();
                    bmax = (*it).getBlue();
                }

                it++;
            }

            ldrfile << rmax << " " << gmax << " " << bmax;
            if (i < numPixAncho-1) {
                ldrfile << "    ";
            }

            //punto por el que queremos pasar
            xLocal += pixelUnit;
        }
        ldrfile << endl;
        yLocal -= pixelUnit;
    }

    ldrfile.close();
}

