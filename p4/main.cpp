#include <string>
#include <fstream>
#include "Plane.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "../p1/Matriz.hpp"
#include <list>
#include <random>


void fromDoubleToRGB(double thr, double thr1, double thr2, int &colour, int &colour1, int &colour2);

int main(int argc, char* argv[]){

    // int pixelRes = stoi(argv[1]); // Número de rayos (?) (1048576 = 1024x1024)
    int pixelRes = 1024*1024; // Número de rayos (?) (1048576 = 1024x1024)
    ofstream ldrfile;
    ldrfile.open(argv[2]);

    int width = 1024,
        height = 1024;


    // --------------------------------------------------Escena
    Sphere sphere1 = Sphere(Punto(0,0,2200), 20.0, 34, 153, 84);    // Verde
    sphere1.esDifuso();
    Sphere sphere2 = Sphere(Punto(20,20,2220), 20.0, 205, 92, 92);  // Roja
    sphere2.esDifuso();
    Sphere planoFoco = Sphere(Punto(70,20,2300),20.0,255,255,255);     // Plano foco
    planoFoco.setFoco(true);
    planoFoco.esEspecular();

    list<Sphere> figuras;
    figuras.push_back(sphere1);
    figuras.push_back(sphere2);
    figuras.push_back(planoFoco);

    list<Punto> focos; // puntuales
    focos.push_back(Punto(2,5,2220));

    // --------------------------------------------------FIN Escena

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
    double xInit, yInit = height/2.0, max,
            xEnd, yEnd, xLocal, yLocal;
    double rmax, gmax, bmax;
    double rThr, gThr, bThr; // Throughput
    Vector dirLocal, dirGlobal;
    double dist, posZ, normalized;
    bool colisiona, puntual;

    //cuantos pixeles tendrá cada lado
    int numPixAncho = width/pixelUnit;
    int numPixAlto = height/pixelUnit;

    ldrfile << "P3" << endl;
    ldrfile << "#MAX=255" << endl;
    ldrfile << numPixAncho << " " << numPixAlto << endl;
    ldrfile << 255 << endl;

    // Procedimiento: izq -> der, arriba -> abajo
    for (int j = 0; j < numPixAlto; j++) {

        xInit = -width/2.0;

        for (int i = 0; i < numPixAncho; i++) {
            xEnd = xInit + pixelUnit;
            yEnd = yInit - pixelUnit;

            //Antialiasing
            int m;
            xLocal = 0.0, yLocal = 0.0;
            for(m=1.0; m<=6.0; m++){
                xLocal += xInit + (double)(rand()) / ((double)(RAND_MAX/(xEnd - xInit)));
                yLocal += yInit + (double)(rand()) / ((double)(RAND_MAX/(yEnd - yInit)));
            }
            xLocal /= m;
            yLocal /= m;

            dist = sqrt(pow(xLocal,2) + pow(yLocal,2));
            posZ = sqrt(pow(dist,2) + pow(front,2));
            normalized = Vector(xLocal, yLocal, posZ).module();
            dirLocal = Vector(xLocal/normalized, yLocal/normalized, 1);

            //Vector con la dirección local a la matriz de proyección
            //de tipo matriz para poder operar con la matriz de cambio de base
            Matriz local = Matriz(dirLocal, 0);

            //cambio de base, de salida tendremos la dirección del vector en coordenadas globales
            Matriz Global = siscam * local;

            r = Rayo(origen, Global.vector());
            rThr = 1, gThr = 1, bThr = 1; 

            int k=0;
            puntual = false;
            Sphere fig;
            do {
                k++;

                max = 3;
                rmax = 1, gmax = 1, bmax = 1;
                colisiona = false;
                
                list<Sphere>::iterator it = figuras.begin();
                while(it != figuras.end()){
                    double res = (*it).interseccion(r);

                    if(res > 0 && res < max){
                        max = res;
                        // rmax = (*it).getRed();
                        // gmax = (*it).getGreen();
                        // bmax = (*it).getBlue();
                        fig = (*it);
                        colisiona = true;
                    }

                    it++;
                }

                if (colisiona) {
                    if(!fig.soyFoco()){
                        // Modifica valor rayo r por el nuevo generado del rebote
                        reboteCamino(r, fig, focos, figuras, rmax, gmax, bmax, puntual);
                    } //else {
                    //     rmax = (*fig).getRed();
                    //     gmax = (*fig).getGreen();
                    //     bmax = (*fig).getBlue();
                    // }

                    rThr *= rmax;
                    gThr *= gmax;
                    bThr *= bmax; 
                }

            } while(!r.hayAbsorcion() && colisiona && !fig.soyFoco() && !puntual);

            if (!colisiona || r.hayAbsorcion()) {
                ldrfile << 0 << " " << 0 << " " << 0;
            } else {
                int rColour, gColour, bColour;

                fromDoubleToRGB(rThr, gThr, bThr, rColour, gColour, bColour);
                // TODO si se pasan de 255 acotar
                ldrfile << rColour << " " << gColour << " " << bColour;
            }
            if (i < numPixAncho-1) {
                ldrfile << "    ";
            }

            //punto por el que queremos pasar
            xInit += pixelUnit;
        }
        ldrfile << endl;
        yInit -= pixelUnit;
    }

    ldrfile.close();
}

void fromDoubleToRGB(double thr, double thr1, double thr2,
                     int &colour, int &colour1, int &colour2) {
    colour = (thr * 255.0);
    colour1 = (thr1 * 255.0);
    colour2 = (thr2 * 255.0);

}
