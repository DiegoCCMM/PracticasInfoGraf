/*
 * Sist_coord.hpp
 *
 *  Created on: 6 oct. 2020
 *      Author: patri
 */

#include "Vector.hpp"

#ifndef SIST_COORD_HPP_
#define SIST_COORD_HPP_

class Sist_coord {
public:
	Vector i, j, k;

	Sist_coord(Vector axis, Vector center_city) {
		i = axis;
		Vector j_aux = i ->* center_city;
		k = i ->* j_aux;
		j = i ->* k;
	}

	Sist_coord(double R, double inclination, double azimuth) {
		/* Longitude tangent direction = (−Rsin(ϕ)sin(θ),Rsin(ϕ)cos(θ),0) */
		i = Vector (-R*sin(azimuth)*sin(inclination),
				    R*sin(azimuth)*cos(inclination),
				    0);

		/* Latitude tangent direction = (Rcos(ϕ)cos(θ),Rcos(ϕ)sin(θ),−Rsin(ϕ))*/
		j = Vector (R*cos(azimuth)*cos(inclination),
				    R*cos(azimuth)*sin(inclination),
				   -R*sin(azimuth));

		/* Surface normal = (Rsin(ϕ)cos(θ),Rsin(ϕ)sin(θ),Rcos(ϕ)) */
		k = Vector (R*sin(azimuth)*cos(inclination),
				    R*sin(azimuth)*sin(inclination),
				    R*cos(azimuth));
    }

};

#endif /* SIST_COORD_HPP_ */
