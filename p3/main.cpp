//
// Created by diego on 7/11/20.
//

#include <string>
#include <fstream>
#include "Plane.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "../p1/Matriz.hpp"



int main(int argc, char* argv[]){

    std::string s(argv[1]);
    std::string out(argv[2]);
    int pixelRes = stoi(s);
    ofstream ldrfile;
    ldrfile.open(out);

    //initializeLDRfile(&ldrfile);

    ldrfile << "P3" << endl;
    ldrfile << "#MAX=255" << endl;
    ldrfile << "15 15" << endl;
    ldrfile << "255" << endl;

    //primera escena
    //
    //Esfera 0,0,10 / 10
    //Cámara 0,0,0 / y = 0,15,0 – z = 0,0,10 – x = 15,0,0
    //Pixeles 144

    //Escena
    Sphere shape1 = Sphere(Punto(0,0,10), 10.0, 255, 0,0);

    //SistCoordenadasCam
    Vector z = Vector(0,0,10);
    Vector y = Vector(0,15,0);
    Vector x = Vector(15,0,0);
    Punto center = Punto(0,0,0);
    //Sistemas de coordenadas en matriz para hacer el cambio de sistemas
    Matriz siscam = Matriz(x,y,z,center);

    //plano de proyección
    double area = 15*15;
    double pixelUnit = area / (double)pixelRes;
    //variables para el for
    Rayo r;
    double xLocal, yLocal, max = -1;
    int rmax = 0,gmax = 0,bmax = 0;
    Vector dirLocal, dirGlobal;
    //cuantos pixeles tendrá cada lado, si hay 3x3 pixeles iteraremos para sacar los 9 rayos en cada pixel
    int howManyPixelsOnSide = 15.0 / pixelUnit;

    for( int f = 1; f <= howManyPixelsOnSide; f++){
        for(int c = 1; c <= howManyPixelsOnSide; c++){
            //punto por el que queremos pasar
            xLocal = (f*pixelUnit)/2;
            yLocal = (c*pixelUnit)/2;
            dirLocal = Vector(xLocal, yLocal, 1);
            //Vector con la dirección local a la matriz de proyección
            //de tipo matriz para poder operar con la matriz de cambio de base
            Matriz local = Matriz(dirLocal, 0);

            //cambio de base, de salida tendremos la dirección del vector en coordenadas globales
            Matriz Global = siscam * local;

            r = Rayo(center, Global.vector());
            //aquí usariamos este rayo para iterar sobre la lista de figuras
            //como sólo tenemos una...
            double res = r.interseccion(shape1);
            if(res > 0 && res > max){
                max = res;
                rmax = shape1.getRed();
                gmax = shape1.getGreen();
                bmax = shape1.getBlue();
            }
            //escribimos en el fichero
            ldrfile << rmax << " " << gmax << " " << bmax << " ";
            rmax = 0;
            gmax = 0;
            bmax = 0;
            max = -1;
        }
    }


}
