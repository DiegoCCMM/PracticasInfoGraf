/*
 * Matriz.hpp
 *
 *  Created on: 6 oct. 2020
 *      Author: patri
 */

#include "Figures_3D.hpp"
#include "Punto.hpp"
#include "Vector.hpp"
#include "iostream"

using namespace std;

#ifndef MATRIZ_HPP_
#define MATRIZ_HPP_

#define DIM 4

class Matriz {
public:
	double matriz[DIM][DIM];	// matriz
	int filas, columnas;			// filas, columnas

	Matriz(int filas, int columnas) {
		this->filas = filas;
		this->columnas = columnas;
	}

	Matriz() {
		this->filas = DIM;
		this->columnas = DIM;
	}

	/* Matriz de transformacion */
	Matriz(Vector i, Vector j, Vector k, Punto o) {
		filas = DIM, columnas = DIM;
		matriz[0][0] = i.x, matriz[0][1] = j.x, matriz[0][2] = k.x, matriz[0][3] = o.x;
		matriz[1][0] = i.y, matriz[1][1] = j.y, matriz[1][2] = k.y, matriz[1][3] = o.y;
		matriz[2][0] = i.z, matriz[2][1] = j.z, matriz[2][2] = k.z, matriz[2][3] = o.z;
		matriz[3][0] = 0,   matriz[3][1] = 0,   matriz[3][2] = 0,   matriz[3][3] = 1;
	}

	/* Matriz de transformacion sin punto*/
	Matriz(Vector i, Vector j, Vector k) {
		filas = 3, columnas = 3;
		matriz[0][0] = i.x, matriz[0][1] = j.x, matriz[0][2] = k.x;
		matriz[1][0] = i.y, matriz[1][1] = j.y, matriz[1][2] = k.y;
		matriz[2][0] = i.z, matriz[2][1] = j.z, matriz[2][2] = k.z;
	}

	/* n --> Punto(1) o direccion(0) */
	Matriz(Figures_3D f, int n) {
		filas = DIM, columnas = 1;
		matriz[0][0] = f.x;
		matriz[1][0] = f.y;
		matriz[2][0] = f.z;
		matriz[3][0] = n;
	}

	/* Matriz pixel resolution scaling */
	void resolution(double width, double height) {
		filas = DIM, columnas = DIM;
		matriz[0][0] = width/2.0, matriz[0][1] = 0, matriz[0][2] = 0, matriz[0][3] = width/2.0;
		matriz[1][0] = 0, matriz[1][1] = height/2.0, matriz[1][2] = 0, matriz[1][3] = height/2.0;
		matriz[2][0] = 0, matriz[2][1] = 0, matriz[2][2] = 0, matriz[2][3] = 0;
		matriz[3][0] = 0, matriz[3][1] = 0, matriz[3][2] = 0, matriz[3][3] = 0;
	}

	/* Matriz perspectiva */	
	void perspectiva() {
		filas = DIM, columnas = DIM;
		matriz[0][0] = 1, matriz[0][1] = 0, matriz[0][2] = 0, matriz[0][3] = 0;
		matriz[1][0] = 0, matriz[1][1] = 1, matriz[1][2] = 0, matriz[1][3] = 0;
		matriz[2][0] = 0, matriz[2][1] = 0, matriz[2][2] = 1, matriz[2][3] = 0;
		matriz[3][0] = 0, matriz[3][1] = 0, matriz[3][2] = 1, matriz[3][3] = 0;
	}


	void setNum(int i, int j, double valor){
		matriz[i][j] = valor;
	}

	Matriz operator-(const Matriz derecha){
		if(columnas != derecha.filas) {
			cout << "Matrices incompatibles" << endl;
		}

		Matriz sol(filas, columnas);

		for(int i=0; i<filas; i++) {
			for(int j=0; j<derecha.columnas; j++) {
				sol.matriz[i][j] = matriz[i][j] - derecha.matriz[i][j];
			}
		}

		return sol;
	}

	Matriz operator+(const Matriz derecha){
		if(columnas != derecha.filas) {
			cout << "Matrices incompatibles" << endl;
		}

		Matriz sol(filas, columnas);

		for(int i=0; i<filas; i++) {
			for(int j=0; j<derecha.columnas; j++) {
				sol.matriz[i][j] = matriz[i][j] + derecha.matriz[i][j];
			}
		}

		return sol;
	}

	Matriz operator*(const Matriz derecha) {
		if(columnas != derecha.filas) {
			cout << "Matrices incompatibles" << endl;
		}

		Matriz sol(filas, derecha.columnas);

		for(int i=0; i<filas; i++) {
			for(int j=0; j<derecha.columnas; j++) {
				sol.matriz[i][j] = 0;
				for(int k=0; k<columnas; k++) {
					sol.matriz[i][j] = sol.matriz[i][j] + this->matriz[i][k] * derecha.matriz[k][j];
				}
			}
		}

		return sol;
	}

	Punto punto() {
		return {matriz[0][0], matriz[1][0], matriz[2][0]};
	}

	Vector vector() {
		return {matriz[0][0], matriz[1][0], matriz[2][0]};
	}

	Vector vector2() {
		return {matriz[0][0]/matriz[3][0], matriz[1][0]/matriz[3][0], matriz[2][0]/matriz[3][0]};
	}
};


#endif /* MATRIZ_HPP_ */
