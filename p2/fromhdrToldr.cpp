//
// Created by diego on 20/10/20.
//

#include <iostream>
#include <fstream>
#include <string>
#include "fromhdrToldr.hpp"

fromhdrToldr::fromhdrToldr(const string output, const string  input)
{
    hdrfile.open(input);
    ldrfile.open(output);

    string line;
    if (ldrfile.is_open())
    {
        while ( getline(hdrfile,line) )
        {

        }
        ldrfile.close();
    }

}
