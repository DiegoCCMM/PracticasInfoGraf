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

    Sist_coord sist_planeta (axis, city-center);

    Matriz T_planet (sist_planeta.i, sist_planeta.j, sist_planeta.k, center);
	Matriz v_city (city, 1);
    Punto position = (T_planet*v_city).punto();

    Sist_coord sist_station (R, inclination, azimuth);

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


	bool correcto1 = datos_correctos(axis1, center1, city1);
	bool correcto2 = datos_correctos(axis2, center2, city2);

	/* ------------------- APARTADO 3 -------------------*/
	if(!correcto1 || !correcto2) {
		cout << "Datos introducidos incorrectos." << endl;
		//return 1;		DESCOMENTAR******************************
	}


	double R1 = (city1-center1).module(); // Radio1
	double R2 = (city2-center2).module(); // Radio2

	/*
	 * Si un vector atravesase un planeta, el eje k de ese vector
	 * respecto al eje de coordenadas de ese sistema debería ser
	 * negativo. Por lo tanto, para poder haber una conexión entre
	 * ambas estaciones, el eje k debe ser positivo desde el punto
	 * de vista en ambos sistemas.
	 */
	Sist_coord sist_planeta1 (axis1, city1-center1);
	Sist_coord sist_planeta2 (axis2, city2-center2);

	Matriz T1_planet (sist_planeta1.i, sist_planeta1.j, sist_planeta1.k, center1);
	Matriz v1_city (city1, 1);
	Punto position1 = (T1_planet*v1_city).punto();

	Matriz T2_planet (sist_planeta2.i, sist_planeta2.j, sist_planeta2.k, center2);
	Matriz v2_city (city2, 1);
	Punto position2 = (T2_planet*v2_city).punto();

	Sist_coord sist_station1 (R1, inclination1, azimuth1);
	Sist_coord sist_station2 (R2, inclination2, azimuth2);

	Vector connection (position2-position1);

	/* Se transforma el vector de la conexión respecto cada uno de los
	 * sistemas de coordenadas.
	 */
	Matriz T1_station (sist_station1.i, sist_station1.j, sist_station1.k, city1),
		   T2_station (sist_station2.i, sist_station2.j, sist_station2.k, city2);
	Matriz v_conn (connection, 0);
	Vector sist_con1 = (T1_station*v_conn).vector();
	Vector sist_con2 = (T2_station*v_conn).vector();

	if(conexion_correcta(sist_con1.z, sist_con2.z)) {
		cout << "La conexion se ha realizado correctamente." << endl;
	}
	else cout << "Error. Uno o los dos planetas interfieren en la conexion." << endl;

    return 0;

}
