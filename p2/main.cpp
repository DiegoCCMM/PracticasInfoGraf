#include <iostream>
#include "fromhdrToldr.hpp"

int main(int argc, char *argv[]) {

    fromhdrToldr operacion(argv[0], argv[1]);
    operacion.readWrite();

}
