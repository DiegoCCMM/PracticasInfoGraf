//
// Created by diego on 20/10/20.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "fromhdrToldr.hpp"

fromhdrToldr::fromhdrToldr(const string& input, const string&  output)
{
    hdrfile.open(input);
    ldrfile.open(output);

    string line;
    if (ldrfile.is_open())
    {
    	getline(hdrfile,line); //P3
    	ldrfile << line << endl;

        getline(hdrfile,line); //#MAX=48
        ldrfile << line << endl;

        string sMax = line.substr(line.find("=")+1);
        Max = stof(sMax);

        getline(hdrfile,line); //# forest_path.ppm
        ldrfile << line << endl;

        getline(hdrfile,line);    //2048 1536
        ldrfile << line << endl;
        width = stoi(line.substr(0, line.find(' ')));
        height = stoi(line.substr(line.find(' ')+1));

        getline(hdrfile,line);    //10000000
        ldrfile << line << endl;
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

                if(data>255) fdata = 1.0; //(1)
                else fdata = (float)(data * Max) / (float)resInColorSpace;

                fdata = (float) resInColorSpace * pow((fdata / (float)resInColorSpace), 0.5); //gamma

                data = (fdata * resInColorSpace) / Max; //(2)

                ldrfile << data;
                if (i % 3 != 0) {
                    ldrfile << " ";
                } else {
                    ldrfile << "    ";
                }
            }
            ldrfile << endl;
        }

    }

}
