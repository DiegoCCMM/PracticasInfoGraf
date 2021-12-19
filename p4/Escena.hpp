/*
 *    Author:         Patricia Briones Yus, 735576
 *    Creation Date:  19 Diciembre 2021
 *    File: Escena.hpp
 */
#include <list>
#include "FocoPuntual.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

#ifndef Escena_hpp
#define Escena_hpp


class Escena {
public:
   list<geometryRGBFigures*> figuras;
   list<FocoPuntual> focos; // Luces puntuales
   int width, height;

   Escena(int _width, int _height){
      width = _width; height = _height;
   }

   void escena1(){
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
   //  figuras.push_back(&planoFoco6);


    focos.push_back(FocoPuntual(Punto(0,0,400), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(5,5,450), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(10,10,450), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(0,-5,450), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(5,-5,450), 255, 255, 255));
    // focos.push_back(FocoPuntual(Punto(-5,-5,450), 255, 255, 255));

    // --------------------------------------------------FIN Escena
   } 

};

#endif //Escena_hpp