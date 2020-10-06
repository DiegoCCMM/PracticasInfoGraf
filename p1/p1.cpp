//============================================================================
// Name        : p1.cpp
// Author      : 735576,
// Version     : 1
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>

#include "Vector.hpp"
#include "Punto.hpp"

using namespace std;

int main() {

    Vector a = Vector(1,2,3);
    Punto city = Punto(1,2,3);
    Punto center = Punto(2,3,4);

    Vector v =  center - city;
    cout << v.module() << endl;

}