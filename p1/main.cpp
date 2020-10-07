#include <iostream>

#include "Vector.hpp"
#include "Punto.hpp"
#include "Matriz.hpp"
#include "Sist_coord.hpp"

using namespace std;

bool datos_correctos(Vector axis, Punto center, Punto city){
    return axis.module()/2 == (city-center).module();
}

bool conexion_correcta(double a, double b){
	return a >= 0 && b >= 0;
}


int main() {

    /* DATOS INICIALES */
    Vector axis   (12,0,2.1);
    Punto  center (8.3,-5,3.5);
    Punto  city   (9.6,-4.5,5.6);

    /* ------------------- APARTADO 2 -------------------*/
    bool correcto = datos_correctos(axis, center, city);

    /* ------------------- APARTADO 3 -------------------*/
    if(!correcto) {
        cout << "Datos introducidos incorrectos." << endl;
        //return 1;		DESCOMENTAR******************************
    }

    /* DATOS */

    /*
     * BIBLIOGRAFÍA
     *
     * https://es.wikipedia.org/wiki/Coordenadas_esféricas
     * https://mathinsight.org/parametrized_surface_orient
     */

    double  inclination = M_PI/2,		// θ Range: (0,PI)
    		azimuth = M_PI;				// ϕ Range: (-PI,PI]
    double R = (city-center).module(); 	// Radio

    Punto position (R, inclination, azimuth);
    Sist_coord sistema (R, inclination, azimuth);

    /* ------------------- APARTADO 4 -------------------*/

    /* DATOS PRIMERA ESTACIÓN (1)*/
    Vector axis1   (12,0,2.1);
	Punto  center1 (8.3,-5,3.5);
	Punto  city1   (9.6,-4.5,5.6);
	double  inclination1 = M_PI/2,		// θ1 Range: (0,PI)
			azimuth1 = M_PI;			// ϕ1 Range: (-PI,PI]

	/* DATOS SEGUNDA ESTACIÓN (2)*/
	Vector axis2   (12,0,2.1);
	Punto  center2 (8.3,-5,3.5);
	Punto  city2   (9.6,-4.5,5.6);
	double  inclination2 = M_PI/2,		// θ2 Range: (0,PI)
			azimuth2 = M_PI;			// ϕ2 Range: (-PI,PI]


	double R1 = (city1-center1).module(); // Radio1
	double R2 = (city2-center2).module(); // Radio2

	/*
	 * Si un vector atravesase un planeta, el eje k de ese vector
	 * respecto al eje de coordenadas de ese sistema debería ser
	 * negativo. Por lo tanto, para poder haber una conexión entre
	 * ambas estaciones, el eje k debe ser positivo desde el punto
	 * de vista en ambos sistemas.
	 */
	Punto position1 (R1, inclination1, azimuth1);
	Punto position2 (R2, inclination2, azimuth2);

	Sist_coord sistema1 (R1, inclination1, azimuth1);
	Sist_coord sistema2 (R2, inclination2, azimuth2);

	Vector connection (position2-position1); // Podría ser: city2-city1 (???????)

	/* Se transforma el vector de la conexión respecto cada uno de los
	 * sistemas de coordenadas.
	 */
	Matriz T1 (sistema1.i, sistema1.j, sistema1.k, position1),
		   T2 (sistema2.i, sistema2.j, sistema2.k, position2);
	Matriz v (connection, 0);
	Vector sist_con1 = (T1*v).vector();
	Vector sist_con2 = (T2*v).vector();

	if(conexion_correcta(sist_con1.z, sist_con2.z)) {
		cout << "La conexion se ha realizado correctamente." << endl;
	}
	else cout << "Error. Uno o los dos planetas interfieren en la conexion." << endl;

    return 0;

}
