#include <iostream>

#include "Vector.hpp"
#include "Punto.hpp"
#include "Matriz.hpp"

using namespace std;

bool datos_correctos(Vector axis, Punto center, Punto city){
    return axis.module()/2 == (city-center).module();
}


int main() {

    /* DATOS INICIALES */
    Vector axis   (12,0,2.1);
    Punto  center (8.3,-5,3.5);
    Punto  city   (9.6,-4.5,5.6);

    /* APARTADO 2 */
    bool correcto = datos_correctos(axis, center, city);

    /* APARTADO 3 */
    if(!correcto) {
        cout << "Datos incorrectos" << endl;
        //return 1;
    }

    /* DATOS */
    double  inclination = M_PI/2,	// Range: (0,PI)
    azimuth = M_PI;			// Range: (-PI,PI]

    // Se genera el sistema de coordenadas
    Vector i = axis;
    Vector j = city - center;	// perpendicular a i --> el producto vectorial es 0
    Vector k = i ->* j;		// perpendicular a i y j --> i x j = k

    Matriz T(i,j,k,center), v(city, 1);

    Punto position = (T*v).punto();

    //Vector surface_normal = ;

    return 0;

}