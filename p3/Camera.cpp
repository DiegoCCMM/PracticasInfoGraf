//
// Created by diego on 8/11/20.
//

#include "Camera.hpp"

Camera::Camera(const Sist_coord &sistCoordCam, const Punto &origen) : sistCoordCam(sistCoordCam), origen(origen) {}

Rayo::Rayo(const Punto &origen, const Vector &direccion) : origen(origen), direccion(direccion) {}

double Rayo::interseccion(Plane plano) {
    return(-(plano.getDistance() + this->origen * plano.getNormal())/(this->direccion * plano.getNormal()));


}

double Rayo::interseccion(Sphere esfera) {

    //a = mod(d) ^ 2 / b = 2d*(o-c) / c = mod(o - c)^2 - r ^ 2
    return(secGradeEcuation(pow(this->direccion.module(), 2),
                            2*((this->origen - esfera.getCenter()) * this->direccion),
                            pow((this->origen - esfera.getCenter()).module(),2) - pow(esfera.getRadius(), 2)));
}

double Rayo::secGradeEcuation(double a, double b, double c) {
    if(pow(b,2) - 4 * a * c >= 0)
    {
        double primeraSol = (-b - sqrt(pow(b,2) - 4 * a * c)) / (2*a);
        double segundaSol = (-b + sqrt(pow(b,2) - 4 * a * c)) / (2*a);

        if(primeraSol < segundaSol){
            return (primeraSol);
        }else{
            return(segundaSol);
        }

    }else{return -1;}

}
