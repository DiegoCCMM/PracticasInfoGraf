//
// Created by diego on 8/11/20.
//

#include "../p1/Sist_coord.hpp"
#include "../p1/Punto.hpp"

#ifndef P3_CAMERA_HPP
#define P3_CAMERA_HPP


class camera {
    Sist_coord sistCoordCam;
    Punto origen;
public:
    camera(const Sist_coord &sistCoordCam, const Punto &origen);
};


#endif //P3_CAMERA_HPP
