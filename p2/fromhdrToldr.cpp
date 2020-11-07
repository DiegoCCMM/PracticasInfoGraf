//
// Created by diego on 20/10/20.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "fromhdrToldr.hpp"

/*
* c --> resInColorSpace
* v --> valor real guardado en memoria
* m --> máximo valor de la imagen (Max)
* s --> tupla con el valor de R, G, B
*/

fromhdrToldr::fromhdrToldr(const string& input, const string&  output)
{
    hdrfile.open(input);
    ldrfile.open(output);

    string line;
    if (ldrfile.is_open() && hdrfile.is_open())
    {
    	getline(hdrfile,line); //P3
    	ldrfile << line << endl;

        getline(hdrfile,line); //#MAX=48
        ldrfile << "#MAX=255" << endl;

        string sMax = line.substr(line.find("=")+1);
        Max = stof(sMax);

        getline(hdrfile,line); //# forest_path.ppm
        ldrfile << line << endl;

        getline(hdrfile,line);    //2048 1536
        ldrfile << line << endl;
        width = stoi(line.substr(0, line.find(' ')));
        height = stoi(line.substr(line.find(' ')+1));

        getline(hdrfile,line);    //10000000
        ldrfile << 255 << endl;
        resInColorSpace = stoi(line);

    }
}

void fromhdrToldr::readWrite() {

    int s1,s2,s3;
    float fdata, v1, v2, v3;

    if (hdrfile.is_open() && ldrfile.is_open()){
        for(int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                hdrfile >> s1 >> s2 >> s3;
                // Equalization m = 1
                v1 = (float)s1/(float)resInColorSpace;
                v2 = (float)s2/(float)resInColorSpace;
                v3 = (float)s3/(float)resInColorSpace;

                // Para tener valores entre 0 y 1
                v1 = v1/Max;
                v2 = v2/Max;
                v3 = v3/Max;

                // Gamma curve
                v1 = pow(v1,(float)0.555);
                v2 = pow(v2,(float)0.555);
                v3 = pow(v3,(float)0.555);

                s1 = v1*255*Max;
                s2 = v2*255*Max;
                s3 = v3*255*Max;

                // Clamping
                if(s1>255) s1 = 255;
                if(s2>255) s2 = 255;
                if(s3>255) s3 = 255;

                ldrfile << s1 << " " << s2 << " " << s3 << " ";
                if (i < width-1) {
                    ldrfile << "    ";
                }
            }
            ldrfile << endl;
        }

    }
    hdrfile.close();
    ldrfile.close();
}
