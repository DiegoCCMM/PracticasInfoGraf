#include <string>
#include <fstream>
#include "Plane.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "FocoPuntual.hpp"
#include "../p1/Matriz.hpp"
#include <list>
#include <limits.h>
#include <random>


void fromDoubleToRGB(RGB thr, RGB &rgb) {

    rgb = thr * 255.0;

    if (rgb.r > 255) rgb.r = 255;
    if (rgb.g > 255) rgb.g = 255;
    if (rgb.b > 255) rgb.b = 255;
}

int main(int argc, char* argv[]){
    // El tamanyo de la imagen ha de ser cuadrado
    int width = 480,
        height = 480;
    // Antialiasing
    int rperPixel = 10.0;

    
    int pixelRes = width*height; // Número de rayos (?) (1048576 = 1024x1024)
    ofstream ldrfile;
    ldrfile.open("figureLDR.ppm");



    // --------------------------------------------------Escena
    Sphere sphere1 = Sphere(Punto(-5,0,460), 2.0);
    // Sphere sphere1 = Sphere(Punto(10,0,2000), 10.0, 255, 255, 255);   // blanca
    // sphere1.esDifuso();
    sphere1.esEspecular();
    // sphere1.setFoco(true);
    // sphere1.esDielectrico();*/

    Sphere sphere2 = Sphere(Punto(5,0,460), 2.0);
    sphere2.esDielectrico();
    // sphere2.esEspecular();
    // sphere2.esDifuso();
    // sphere2.setFoco(true);

    Sphere sphere3 = Sphere(Punto(5,0,460), 2.0, 235, 23, 181); // rosa
    // sphere3.setFoco(true);
    // sphere3.esEspecular();
    sphere3.esDifuso();
    // sphere3.esDielectrico();

   /* Sphere sphere4 = Sphere(Punto(-10,0,2100), 8.0, 200, 200, 200); // Pelota detrás
    sphere4.setFoco(true);
    // sphere4.esEspecular();
    sphere4.esDifuso();
    // sphere4.esDielectrico();

    // Esfera foco grande en medio que intenta iluminar la sala
    Sphere sphere5 = Sphere(Punto(0,0,470), 2.0, 0, 0, 255);
    // sphere5.setFoco(true);
    // sphere5.esEspecular();
    sphere5.esDifuso();
    // sphere5.esDielectrico(); */

    // Plano - fondo
    Plane planoFoco1 = Plane(Vector(0,0,-1), Punto(0,0,475), 255, 255, 255);  // Plano foco
    // planoFoco1.setFoco(true);
    // planoFoco.esEspecular();
    planoFoco1.esDifuso();

    // Plano - izquierda
    Plane planoFoco2 = Plane(Vector(1,0,-1), Punto(-width,0,0), 255, 0, 0);  // Plano foco
    // planoFoco2.setFoco(true);
    // planoFoco.esEspecular();
    planoFoco2.esDifuso();

    // Plano - derecha
    Plane planoFoco3 = Plane(Vector(-1,0,-1), Punto(width,0,0), 0, 255, 0);  // Plano foco
    // planoFoco3.setFoco(true);
    // planoFoco.esEspecular();
    planoFoco3.esDifuso();

    // Plano - techo
    //Plane planoFoco4 = Plane(Vector(0,-30,-30), Punto(0,170,2220), 0, 255, 0);  // Plano foco
    Plane planoFoco4 = Plane(Vector(0,-1,-1), Punto(0,height,0), 255, 255,255);  // Plano foco
    // planoFoco4.setFoco(true);
    // planoFoco.esEspecular();
    planoFoco4.esDifuso();

    // Plano - suelo
    Plane planoFoco5 = Plane(Vector(0,1,-1), Punto(0,-height,0), 255, 255, 255);  // Plano foco
    //planoFoco5.setFoco(true);
    // planoFoco.esEspecular();
    planoFoco5.esDifuso();

    // Plano - pared trasera
    Plane planoFoco6 = Plane(Vector(0,0,1), Punto(0,0,300), 255, 255, 255);  // Plano foco
    //planoFoco6.setFoco(true);
    // planoFoco6.esEspecular();
    planoFoco6.esDifuso();

    list<geometryRGBFigures*> figuras;

    // figuras.push_back(&sphere1);
    // figuras.push_back(&sphere2);
    figuras.push_back(&sphere3);
    // figuras.push_back(&sphere4);
    // figuras.push_back(&sphere5);
    figuras.push_back(&planoFoco1);
    figuras.push_back(&planoFoco2);
    figuras.push_back(&planoFoco3);
    figuras.push_back(&planoFoco4);
    figuras.push_back(&planoFoco5);
    figuras.push_back(&planoFoco6);

    list<FocoPuntual> focos; // puntuales
    focos.push_back(FocoPuntual(Punto(6,2,475), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(5,5,450), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(10,10,450), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(0,-5,450), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(5,-5,450), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(-5,-5,450), 255, 255, 255));

    // --------------------------------------------------FIN Escena

    // Sistema de coordenadas de la cámara
    int front = (double)height/(1.0*tan(M_PI/12.0));
    Vector  x = Vector(width/2.0,0,0),
            y = Vector(0,height/2.0,0),
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
    RGB Rebote, Radiance, Throughput;
    double rThr, gThr, bThr; // Throughput
    Vector dirLocal, dirGlobal;
    double dist, dirZ, normalized;
    bool colisiona;

    //cuantos pixeles tendra cada lado
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
                xLocal = 0.0, yLocal = 0.0;
                for(m=1.0; m<=6.0; m++){
                    xLocal += xInit + (double)(rand()) / ((double)(RAND_MAX/(xEnd - xInit)));
                    yLocal += yInit + (double)(rand()) / ((double)(RAND_MAX/(yEnd - yInit)));
                }
                xLocal /= (m-1);
                yLocal /= (m-1);

                dist = sqrt(pow(xLocal,2) + pow(yLocal,2));
                dirZ = sqrt(pow(dist,2) + pow(front,2));
            
                dirLocal = Vector(xLocal, yLocal, dirZ).normalizar();
                

                //Vector con la dirección local a la matriz de proyección
                //de tipo matriz para poder operar con la matriz de cambio de base
                Matriz local = Matriz(dirLocal, 0);

                //cambio de base, de salida tendremos la dirección del vector en coordenadas globales
                Matriz Global = siscam * local;

                r = Rayo(origen, Global.vector());
                Throughput = RGB(1.0);
                

                geometryRGBFigures* fig;
                Radiance = RGB(0.0);
                do {
                    max = INT_MAX;
                    colisiona = false;
                    
                    auto it = figuras.begin();
                    while(it != figuras.end()){
                        
                        double res = (*it)->interseccion(r);

                        if(res >= 0 && res < max){
                            max = res;
                            fig = (*it);
                            colisiona = true;
                        }

                        it++;
                    }

                    if (colisiona) {
                        reboteCamino(r, fig, focos, figuras, Radiance, Throughput);
                        
                    }

                } while(!r.hayAbsorcion() && colisiona && !fig->soyFoco());

                if(fig->soyFoco()){
                    //cogemos Radiance + Throughput
                    Throughput = Throughput + Radiance;
                    
                }else if (!colisiona || (r.hayAbsorcion() && Radiance==0.0)) {
                    Throughput = RGB(0.0);
                    
                }else {
                    //cogemos Radiance
                    Throughput = Radiance;
                    
                }
                
                MediaAntialiasing = MediaAntialiasing + Throughput;
            }

            RGB rgb;

            fromDoubleToRGB(MediaAntialiasing/rperPixel, rgb);
            ldrfile << rgb.r << " " << rgb.g << " " << rgb.b;

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


