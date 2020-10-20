#include <iostream>
#include "fromhdrToldr.hpp"
#include "fromhdrToldr.cpp"

int main(int argc, char *argv[]) {

    fromhdrToldr operacion(argv[1], argv[2]);
    operacion.readWrite();

}
