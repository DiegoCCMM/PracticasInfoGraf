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
