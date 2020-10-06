#include <iostream>

#include "Vector.hpp"
#include "Punto.hpp"
#include "Matriz.hpp"

using namespace std;

bool datos_correctos(Vector axis, Punto center, Punto city){
	cout << (city-center).module() << endl;
	cout << axis.module()/2 << endl;
	return axis.module()/2 == (city-center).module();
}


int main() {

	/* DATOS INICIALES */
	Vector axis   (8,12,16);
	Punto  center (3,19,-4);
	Punto  city   (1,16,-8);

	/* APARTADO 2 */
	bool correcto = datos_correctos(axis, center, city);

	/* APARTADO 3 */
	if(!correcto) {
		cout << "Datos incorrectos" << endl;
		return 1;
	}

	/* DATOS */
	double  inclination = M_PI/2,	// Range: (0,PI)
			azimuth = M_PI;			// Range: (-PI,PI]

	// Se genera el sistema de coordenadas
	Vector i = axis;
	Vector j (-i.y,i.x,0);	// perpendicular a i --> el producto escalar es 0
	Vector k = i ->* j;		// perpendicular a i y j --> i x j = k

	Matriz T(i,j,k), v(city, 1);

	Punto position = (T*v).punto();

	//Vector surface_normal = ;

    return 0;

}
