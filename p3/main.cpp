//
// Created by diego on 7/11/20.
//

#include <string>
#include <fstream>
#include "Plane.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "../p1/Matriz.hpp"

const int Y =  1;
const int X =  1;

struct rgb{
    int red = 0;
    int green = 0;
    int blue = 0;
};

int main(int argc, char* argv[]){

    std::string s(argv[1]);
    std::string out(argv[2]);
    int pixelRes = stoi(s);
    ofstream ldrfile;
    ldrfile.open(out);

    //initializeLDRfile(&ldrfile);

    ldrfile << "P3" << endl;
    ldrfile << "#MAX=255" << endl;
    ldrfile << "2 2" << endl;
    ldrfile << "255" << endl;

    //primera escena
    //
    //Esfera 0,0,10 / 10
    //Cámara 0,0,0 / y = 0,15,0 – z = 0,0,10 – x = 15,0,0
    //Pixeles 144

    //Escena
    Sphere shape1 = Sphere(Punto(0,0,0), 1, 255, 0,0);

    //SistCoordenadasCam
    Vector z = Vector(0,0,10);
    Vector y = Vector(0,Y,0);
    Vector x = Vector(X,0,0);;
    Punto center = Punto(0,0,-10);
    //Sistemas de coordenadas en matriz para hacer el cambio de sistemas
    Matriz siscam = Matriz(x,y,z,center);

    //plano de proyección
    double area = (X*2)*(Y*2);
    double pixelUnit = area / (double)pixelRes;

    //variables para el for
    Rayo r;
    double xLocal, yLocal, max = -1;
    rgb rgbmax;
    Vector dirLocal;

    //cuantos pixeles tendrá cada lado, se asume cuadrado
    int howManyPixelsOnSide = X*2 / pixelUnit;

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
            Matriz dirGlobal = siscam * local;

            r = Rayo(center, dirGlobal.vector());
            //aquí usariamos este rayo para iterar sobre la lista de figuras
            //como sólo tenemos una...
            double res = r.interseccion(shape1);
            if(res > 0 && res > max){
                max = res;
                rgbmax.red = shape1.getRed();
                rgbmax.green = shape1.getGreen();
                rgbmax.blue = shape1.getBlue();
            }
            //escribimos en el fichero
            ldrfile << rgbmax.red << " " << rgbmax.green << " " << rgbmax.blue << " ";

            rgbmax.red = 0;
            rgbmax.green = 0;
            rgbmax.blue = 0;
            max = -1;
        }
    }


}

