//
// Created by diego on 20/10/20.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "fromhdrToldr.hpp"

fromhdrToldr::fromhdrToldr(const string& output, const string&  input)
{
    hdrfile.open(input);
    ldrfile.open(output);

    string line;
    if (ldrfile.is_open())
    {
        getline(hdrfile,line); //#MAX=48

        string sMax = line.substr(1, line.find('='));
        Max = stof(sMax);

        getline(hdrfile,line); //# forest_path.ppm

        getline(hdrfile,line);    //2048 1536
        width = stoi(line.substr(0, line.find(' ')));
        height = stoi(line.substr(1, line.find(' ')));

        getline(hdrfile,line);    //10000000
        resInColorSpace = stoi(line);

    }

}

void fromhdrToldr::readWrite() {

    int data;
    float fdata;
    if (hdrfile.is_open() && ldrfile.is_open()){
        for(int j = 0; j < height; j++) {
            for (int i = 0; i < 3 * width; i++) {
                hdrfile >> data;

                fdata = (data * Max) / resInColorSpace; //(1)

                fdata = resInColorSpace * pow((fdata / resInColorSpace), 1 / 2); //gamma

                data = (fdata * resInColorSpace) / Max; //(2)

                ldrfile << data;
                if (i % 3 != 0) {
                    ldrfile << " ";
                } else {
                    ldrfile << "     ";
                }
            }
            ldrfile << "\n";
        }

    }

}

