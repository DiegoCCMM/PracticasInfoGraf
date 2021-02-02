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
	double A[DIM][DIM];	// matriz
	int N, M;			// filas, columnas

	Matriz(int N, int M) {
		this->N = N;
		this->M = M;
	}

	/* Matriz de transformacion */
	Matriz(Vector i, Vector j, Vector k, Punto o) {
		N = DIM, M = DIM;
		A[0][0] = i.x, A[0][1] = j.x, A[0][2] = k.x, A[0][3] = o.x;
		A[1][0] = i.y, A[1][1] = j.y, A[1][2] = k.y, A[1][3] = o.y;
		A[2][0] = i.z, A[2][1] = j.z, A[2][2] = k.z, A[2][3] = o.z;
		A[3][0] = 0,   A[3][1] = 0,   A[3][2] = 0,   A[3][3] = 1;
	}

	/* Matriz de transformacion sin punto*/
	Matriz(Vector i, Vector j, Vector k) {
		N = 3, M = 3;
		A[0][0] = i.x, A[0][1] = j.x, A[0][2] = k.x;
		A[1][0] = i.y, A[1][1] = j.y, A[1][2] = k.y;
		A[2][0] = i.z, A[2][1] = j.z, A[2][2] = k.z;
	}

	/* n --> Punto(1) o direccion(0) */
	Matriz(Figures_3D f, int n) {
		N = DIM, M = 1;
		A[0][0] = f.x;
		A[1][0] = f.y;
		A[2][0] = f.z;
		A[3][0] = n;
	}

	void setNum(int i, int j, double valor){
		A[i][j] = valor;
	}

	Matriz operator*(const Matriz B) {
		if(M != B.N) {
			cout << "Matrices incompatibles" << endl;
		}

		Matriz sol(N, B.M);

		for(int i=0; i<N; i++) {
			for(int j=0; j<B.M; j++) {
				sol.A[i][j] = 0;
				for(int k=0; k<M; k++) {
					sol.A[i][j] = sol.A[i][j] + A[i][k] * B.A[k][j];
				}
			}
		}

		return sol;
	}

	Punto punto() {
		return {A[0][0], A[1][0], A[2][0]};
	}

	Vector vector() {
		return {A[0][0], A[1][0], A[2][0]};
	}
};


#endif /* MATRIZ_HPP_ */
