#include <string>
#include <fstream>
#include "Plane.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "../p1/Matriz.hpp"
#include <list>
#include <limits.h>
#include <random>


void fromDoubleToRGB(double thr, double thr1, double thr2, int &colour, int &colour1, int &colour2);

int main(int argc, char* argv[]){
    // El tamanyo de la imagen ha de ser cuadrado
    int width = 480,
    // int width = 640,
        height = 480;

    // int rperPixel = stoi(argv[1]);
    // int rperPixel = 4; // Antialiasing
    int rperPixel = 10;

    // int pixelRes = stoi(argv[1]); // Número de rayos (?) (1048576 = 1024x1024)
    int pixelRes = width*height; // Número de rayos (?) (1048576 = 1024x1024)
    ofstream ldrfile;
    // ldrfile.open(argv[2]);
    ldrfile.open("figureLDR.ppm");



    // --------------------------------------------------Escena
    Sphere sphere1 = Sphere(Punto(-5,0,460), 2.0, 235, 23, 181);    // Rosa
    // Sphere sphere1 = Sphere(Punto(10,0,2000), 10.0, 255, 255, 255);   // blanca
    // sphere1.esDifuso();
    sphere1.esEspecular();
    // sphere1.setFoco(true);
    // sphere1.esDielectrico();

    Sphere sphere2 = Sphere(Punto(0,-5,460), 2.0, 0, 0, 255);  // Azul
    // sphere2.esDielectrico();
    // sphere2.esEspecular();
    sphere2.esDifuso();
    // sphere2.setFoco(true);

    Sphere sphere3 = Sphere(Punto(5,0,460), 2.0, 0, 0, 0); // Pelota detrás
    // sphere3.setFoco(true);
    // sphere3.esEspecular();
    // sphere3.esDifuso();
    sphere3.esDielectrico();

    Sphere sphere4 = Sphere(Punto(-10,0,2100), 8.0, 200, 200, 200); // Pelota detrás
    sphere4.setFoco(true);
    // sphere4.esEspecular();
    sphere4.esDifuso();
    // sphere4.esDielectrico();

    // Esfera foco grande en medio que intenta iluminar la sala
    Sphere sphere5 = Sphere(Punto(0,0,470), 2.0, 0, 0, 255);
    // sphere5.setFoco(true);
    // sphere5.esEspecular();
    sphere5.esDifuso();
    // sphere5.esDielectrico();

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
    planoFoco4.setFoco(true);
    // planoFoco.esEspecular();
    planoFoco4.esDifuso();

    // Plano - suelo
    Plane planoFoco5 = Plane(Vector(0,1,-1), Punto(0,-height,0), 255, 255, 255);  // Plano foco
    // planoFoco5.setFoco(true);
    // planoFoco.esEspecular();
    planoFoco5.esDifuso();

    list<geometryRGBFigures*> figuras;

    figuras.push_back(&sphere1);
    figuras.push_back(&sphere2);
    figuras.push_back(&sphere3);
    // figuras.push_back(&sphere4);
    // figuras.push_back(&sphere5);
    figuras.push_back(&planoFoco1);
    figuras.push_back(&planoFoco2);
    figuras.push_back(&planoFoco3);
    figuras.push_back(&planoFoco4);
    figuras.push_back(&planoFoco5);

    list<Punto> focos; // puntuales
    focos.push_back(Punto(2,5,2220));

    // --------------------------------------------------FIN Escena

    // Sistema de coordenadas de la cámara
    int front = (double)height/(1.0*tan(M_PI/12.0));
    // int front = (width/2.0) / (tan(0.26 * M_PI)); // Evitar el ojo de pez
    Vector  x = Vector(width/2.0,0,0),
            y = Vector(0,height/2.0,0),
            z = Vector(0,0,front);
            // NO HACE FALTA QUE ESTEN NORMALIZADOS


    Punto origen = Punto(0,0,0);
    //Sistemas de coordenadas en matriz para hacer el cambio de sistemas
    Matriz siscam = Matriz(x,y,z,origen);
    // Matriz pixel_resol = Matriz();
    // pixel_resol.resolution(width,height);
    // Matriz perspective = Matriz();
    // perspective.perspectiva();

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
    double dist, dirZ, normalized;
    bool colisiona, puntual;

    //cuantos pixeles tendra cada lado
    int numPixAncho = width/pixelUnit;
    int numPixAlto = height/pixelUnit;

    ldrfile << "P3" << endl;
    ldrfile << "#MAX=255" << endl;
    ldrfile << numPixAncho << " " << numPixAlto << endl;
    ldrfile << 255 << endl;
// int numRebotes = 0;
    // Procedimiento pixeles en imagen: izq -> der, arriba -> abajo
    for (int j = 0; j < numPixAlto; j++) {

        xInit = -width/2.0;

        for (int i = 0; i < numPixAncho; i++) {
            xEnd = xInit + pixelUnit;
            yEnd = yInit - pixelUnit;

            double rThrMedia = 0, gThrMedia = 0, bThrMedia = 0;
            // double rThrMedia = 1, gThrMedia = 1, bThrMedia = 1;

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
                // normalized = Vector(xLocal, yLocal, dirZ).module();
                // dirLocal = Vector(xLocal/normalized, yLocal/normalized, 1);
                dirLocal = Vector(xLocal, yLocal, dirZ).normalizar();
                // dirLocal.setZ(1);
                // dirLocal = x.mul(yLocal) + y.mul(xLocal) + z;
                // dirLocal = dirLocal.normalizar();
                // dirLocal.setZ(1);

                //Vector con la dirección local a la matriz de proyección
                //de tipo matriz para poder operar con la matriz de cambio de base
                Matriz local = Matriz(dirLocal, 0);

                //cambio de base, de salida tendremos la dirección del vector en coordenadas globales
                Matriz Global = siscam * local;

                r = Rayo(origen, Global.vector());
                // rThr = 1.0, gThr = 1.0, bThr = 1.0; 
                rThr = 0.0, gThr = 0.0, bThr = 0.0; 
                int numRebotes = 0;

                puntual = false;
                geometryRGBFigures* fig;

/* TODO: INICIO Funcion recursiva rebote camino en material */
                do {
                    max = INT_MAX;
                    rmax = 1.0, gmax = 1.0, bmax = 1.0;
                    colisiona = false;
                    
                    auto it = figuras.begin();
                    while(it != figuras.end()){
                        /*if(typeid(it)==typeid(Sphere)){
                            std::cout << "hey" << std::endl;
                        }*/
                        double res = (*it)->interseccion(r);

                        if(res >= 0 && res < max){
                            max = res;
                            // rThr = (*it)->getRed();
                            // gThr = (*it)->getGreen();
                            // bThr = (*it)->getBlue();
                            // rmax = (*it).getRed();
                            // gmax = (*it).getGreen();
                            // bmax = (*it).getBlue();
                            fig = (*it);
                            colisiona = true;
                        }

                        it++;
                    }

                    if (colisiona) {
                        
                        // if(!fig->soyFoco()){
                            // Modifica valor rayo r por el nuevo generado del rebote
                            reboteCamino(r, fig, focos, figuras, rmax, gmax, bmax, puntual);
                            numRebotes++;
                        // } //else {
                        //     rmax = (*fig).getRed()/255;
                        //     gmax = (*fig).getGreen()/255;
                        //     bmax = (*fig).getBlue()/255;
                        // }

                        // rmax = fig->getRed()*rmax;
                        // gmax = fig->getGreen()*gmax;
                        // bmax = fig->getBlue()*bmax;

                        // rThr *= rmax;
                        // gThr *= gmax;
                        // bThr *= bmax;
                        // double kd, ks, kt, prAbs = rayo.getAbsorcion();

                        if(numRebotes==1){
                            rThr = rmax, gThr = gmax, bThr = bmax; 
                        }
                        else {
                            rThr *= rmax;
                            gThr *= gmax;
                            bThr *= bmax;
                        }

                        /*if(numRebotes==1){
                            rThr = rmax, gThr = gmax, bThr = bmax; 
                        }
                        else {
                            //TODO SI LA LUZ ES 0 0 255 PERDEMOS INFORMACIÓN

                            if(rmax != 0) rThr *= rmax;
                            if(gmax != 0) gThr *= gmax;
                            if(bmax != 0) bThr *= bmax; 
                        }*/

                        /*if(numRebotes==1){
                            rThr = rmax, gThr = gmax, bThr = bmax; 
                        }
                        else {

                            rThr = rThr + rThr*rmax;
                            if(rThr==0) {
                                rThr = rThr + rThr*rmax;
                            }
                            else {
                                rThr = rThr + rThr*rmax;
                            }
                            gThr = gThr + gThr*gmax;
                            bThr = bThr + bThr*bmax;
                        }*/


                        // rThr += rmax;
                        // gThr += gmax;
                        // bThr += bmax;

                        // rThr += rmax/numRebotes;
                        // gThr += gmax/numRebotes;
                        // bThr += bmax/numRebotes;
                    }

                } while(!r.hayAbsorcion() && colisiona && !fig->soyFoco() && !puntual);



                if (!colisiona || r.hayAbsorcion()) {
                    // ldrfile << 0 << " " << 0 << " " << 0;
                    rThr = 0.0;
                    gThr = 0.0;
                    bThr = 0.0;
                }
/* FIN  Funcion recursiva rebote camino en material */


                rThrMedia += rThr, gThrMedia += gThr, bThrMedia += bThr;
                // rThrMedia *= rThr/numRebotes, gThrMedia *= gThr/numRebotes, bThrMedia *= bThr/numRebotes;
                // rThrMedia += rThr/numRebotes, gThrMedia += gThr/numRebotes, bThrMedia += bThr/numRebotes;
            }

            int rColour, gColour, bColour;

            fromDoubleToRGB(rThrMedia/rperPixel, gThrMedia/rperPixel, 
                            bThrMedia/rperPixel, rColour, gColour, bColour);
            // fromDoubleToRGB(rThrMedia*pow(M_PI,2)/rperPixel, gThrMedia*pow(M_PI,2)/rperPixel, 
            //                 bThrMedia*pow(M_PI,2)/rperPixel, rColour, gColour, bColour);
            // fromDoubleToRGB(rThrMedia*pow(M_PI,2)/numRebotes, gThrMedia*pow(M_PI,2)/numRebotes, 
            //                 bThrMedia*pow(M_PI,2)/numRebotes, rColour, gColour, bColour);
            ldrfile << rColour << " " << gColour << " " << bColour;

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

    if (colour > 255) colour = 255;
    if (colour1 > 255) colour1 = 255;
    if (colour2 > 255) colour2 = 255;
}
