//
// Created by diego on 23/1/21.
//

#ifndef P4_MATERIAL_HPP
#define P4_MATERIAL_HPP

class Material{

    double kd, ks, kt;

    void rayosSecundarios(){
        //TODO fr(x, wi, w0) = kd/pi + ks(x, w0)(delta wr(wi) / n*wi) + kt(x,w0)(delta wt(wi)/n*wi)
        //TODO delta wr = 2n(n*wi)-wi
        //TODO delta wt = arcsin((n0 * sin(w0)) / n1)
        //TODO https://es.wikipedia.org/wiki/%C3%8Dndice_de_refracci%C3%B3n
        //TODO vidrio 1,45 aire 1
    }

};

#endif //P4_MATERIAL_HPP
