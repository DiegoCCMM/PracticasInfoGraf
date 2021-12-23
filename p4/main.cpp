#include <string>
#include <fstream>
#include "Plane.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "../p1/Matriz.hpp"
#include <limits.h>
#include <random>


void fromDoubleToRGB(RGB thr, RGB &rgb) {

    rgb = thr * 255.0;

    if (rgb.r > 255) rgb.r = 255;
    if (rgb.g > 255) rgb.g = 255;
    if (rgb.b > 255) rgb.b = 255;
}

int main(int argc, char* argv[]){
    // El tamaño de la imagen ha de ser cuadrado TODO: cambiar esto?
    int width = 480,
        height = 480;
    
    int rperPixel = 10.0; // Antialiasing
    
    int pixelRes = width*height; // Número de rayos totales
    ofstream ldrfile;
    ldrfile.open("figureLDR.ppm");

    // --------------------------------------------------Escena

    list<geometryRGBFigures*> figuras;
    list<FocoPuntual> focos; // Luces puntuales

    // Cornel box normal, de paredes difusas

    // Plano - izquierda
    Plane izquierda = Plane(Vector(1,0,0), Punto(-30,0,0), 200, 0, 0);  // Plano foco
    izquierda.setFoco(true);
    izquierda.esDifuso();
    figuras.push_back(&izquierda);

    // Plano - derecha
    Plane derecha = Plane(Vector(-1,0,0), Punto(30,0,0), 0, 200, 0);  // Plano foco
    derecha.setFoco(true);
    derecha.esDifuso();
    figuras.push_back(&derecha);

    // Plano - techo
    Plane techo = Plane(Vector(0,-1,0), Punto(0,30,0), 170, 170, 170);  // Plano foco
    techo.setFoco(true);
    techo.esDifuso();
    figuras.push_back(&techo);

    // Plano - suelo
    Plane suelo = Plane(Vector(0,1,0), Punto(0,-30,0), 170, 170, 170);  // Plano foco
    // suelo.setFoco(true);
    suelo.esDifuso();
    figuras.push_back(&suelo);

    // Plano - fondo
    Plane fondo = Plane(Vector(0,0,-1), Punto(0,0,15), 65, 170, 72);  // Plano foco
    // fondo.setFoco(true);
    fondo.esDifuso();
    figuras.push_back(&fondo);

    // Esferas

    Sphere sphere1 = Sphere(Punto(-7,0,10), 6.0, 0, 0, 0);
    // sphere1.esDifuso();
    sphere1.esEspecular();
    // sphere1.esDielectrico();
    // sphere1.setFoco(true);
    figuras.push_back(&sphere1);

    Sphere sphere2 = Sphere(Punto(7,0,10), 6.0, 235, 23, 181); // Rosa
    // sphere2.esDifuso();
    // sphere2.esEspecular();
    sphere2.esDielectrico();
    // sphere2.setFoco(true);
    figuras.push_back(&sphere2);
    
    // Focos puntuales

    // focos.push_back(FocoPuntual(Punto(7,-10,10), 60, 60, 60));
    // focos.push_back(FocoPuntual(Punto(-7,-10,10), 100, 100, 100));
    focos.push_back(FocoPuntual(Punto(0,0,10), 100, 100, 100));

    // --------------------------------------------------FIN Escena

    // Sistema de coordenadas de la cámara
    int front = (double)height/(1.0*tan(M_PI/12.0));
    Vector  x = Vector(width/2.0,0,0),
            y = Vector(0,height/2.0,0),
            z = Vector(0,0,front);

    // Plano - trasero
    Plane trasero = Plane(Vector(0,0,1), Punto(0,0,-front), 255, 255, 255);  // Plano foco
    // trasero.setFoco(true);
    trasero.esDifuso();
    figuras.push_back(&trasero);

    Punto origen = Punto(0,0,-front/1.4);
    // Sistemas de coordenadas en matriz para hacer el cambio de sistemas
    Matriz siscam = Matriz(x,y,z,origen);
    

    // Plano de proyección
    double area = width*height;
    double pixelUnit = area / (double)pixelRes; // medidas de cada pixel

    // Variables para llevar las coordenadas
    double xInit, yInit = height/2.0,
            xEnd, yEnd;

    // Cuantos pixeles tendra cada lado
    int numPixAncho = width/pixelUnit;
    int numPixAlto = height/pixelUnit;

    ldrfile << "P3" << endl;
    ldrfile << "#MAX=255" << endl;
    ldrfile << numPixAncho << " " << numPixAlto << endl;
    ldrfile << 255 << endl;

    // Procedimiento pixeles en imagen: izq -> der, arriba -> abajo
    for (int j = 0; j < numPixAlto; j++) {

        xInit = -width/2.0;

        for (int i = 0; i < numPixAncho; i++) {
            xEnd = xInit + pixelUnit;
            yEnd = yInit - pixelUnit;

            RGB MediaAntialiasing(0.0);

            for(int w=0; w<rperPixel; w++) { // Antialiasing

                int m;
                double xLocal = 0.0, yLocal = 0.0;
                for(m=1.0; m<=6.0; m++){
                    xLocal += xInit + (double)(rand()) / ((double)(RAND_MAX/(xEnd - xInit)));
                    yLocal += yInit + (double)(rand()) / ((double)(RAND_MAX/(yEnd - yInit)));
                }
                xLocal /= (m-1);
                yLocal /= (m-1);

                // double dist = sqrt(pow(xLocal,2) + pow(yLocal,2));
                // double dirZ = sqrt(pow(dist,2) + pow(front,2));

                Punto puntoLocal = Punto(xLocal, yLocal, front/2);                

                // Vector con la dirección local a la matriz de proyección
                // de tipo matriz para poder operar con la matriz de cambio de base
                Matriz local = Matriz(puntoLocal, 1);

                // Cambio de base, de salida tendremos la dirección del vector en coordenadas globales
                Matriz Global = siscam * local;
                Punto puntoGlobal = Global.punto();

                Rayo rayo = Rayo(origen, (puntoGlobal - origen).normalizar());

                MediaAntialiasing = MediaAntialiasing + colorCamino(focos, figuras, rayo);
            }

            RGB rgb;

            fromDoubleToRGB(MediaAntialiasing/rperPixel, rgb);
            ldrfile << (int)rgb.r << " " << (int)rgb.g << " " << (int)rgb.b;

            if (i < numPixAncho-1) {
                ldrfile << "    ";
            }

            // Punto por el que queremos pasar
            xInit += pixelUnit;
        }
        ldrfile << endl;
        yInit -= pixelUnit;
    }

    ldrfile.close();
}