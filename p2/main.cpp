#include <iostream>
#include "fromhdrToldr.hpp"

int main(int argc, char *argv[]) {

    fromhdrToldr operacion(argv[1], argv[2]);
    operacion.readWrite();
}
