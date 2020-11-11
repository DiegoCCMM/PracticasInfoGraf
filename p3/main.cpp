//
// Created by diego on 7/11/20.
//

#include <string>
#include <fstream>
#include "Plane.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "../p1/Matriz.hpp"


void initializeLDRfile(ofstream ofstream);

int main(int argc, char* argv[]){

    std::string s = argv[1];
    std::string out = argv[2];
    int pixelRes = stoi(s);
    ofstream ldrfile;
    ldrfile.open(out);

    initializeLDRfile(ldrfile);

    //primera escena
    //
    //Esfera 0,0,10 / 10
    //Cámara 0,0,0 / y = 0,15,0 – z = 0,0,10 – x = 15,0,0
    //Pixeles 144

    //Escena
    Sphere shape1 = Sphere(Punto(0,0,0), 10.0, 255, 0,0);

    //SistCoordenadasCam
    Vector z = Vector(0,0,10);
    Vector y = Vector(0,15,0);
    Vector x = Vector(15,0,0);
    Punto center = Punto(0,0,0);

    Matriz siscam = Matriz(x,y,z,center);

    Camera camera = Camera(siscam, center);

    //plano de proyección
    double area = 15*15;
    double pixelUnit = area / (double)pixelRes;

    Rayo r;
    double xLocal, yLocal, max = -1;
    int rmax = 0,gmax = 0,bmax = 0;
    Vector dirLocal, dirGlobal;
    int howManyPixelsOnSide = 15.0 / pixelUnit;

    for( int f = 1; f <= howManyPixelsOnSide; f++){
        for(int c = 1; c <= howManyPixelsOnSide; c++){
            //punto por el que queremos pasar
            xLocal = (f*pixelUnit)/2;
            yLocal = (c*pixelUnit)/2;
            dirLocal = Vector(xLocal, yLocal, 1);
            Matriz local = Matriz(dirLocal, 0);

            Matriz Global = siscam * local;

            r = Rayo(center, Global.vector());
            //aquí usariamos este rayo para iterar sobre la lista de figuras
            //como sólo tenemos una...
            double res = r.interseccion(shape1);
            if(res > -1 && res > max){
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

void initializeLDRfile(ofstream ofstream) {

}
