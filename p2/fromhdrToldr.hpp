//
// Created by diego on 20/10/20.
//
#include <iostream>
#include <fstream>
#include <string>
#ifndef P2_FROMHDRTOLDR_HPP
#define P2_FROMHDRTOLDR_HPP

using namespace std;

class fromhdrToldr {
    ofstream ldrfile;
    ifstream hdrfile;

    int width, height, resInColorSpace;
    float Max;

public:
    fromhdrToldr(const string& hdrfile, const string& ldrfile);
    void readWrite();


};


#endif //P2_FROMHDRTOLDR_HPP
