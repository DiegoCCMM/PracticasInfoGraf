#include <iostream>
#include <vector>
#include "fromhdrToldr.hpp"

int main(int argc, char *argv[]) {

    std::vector<std::string> args(argv, argv + argc);

    fromhdrToldr operacion(args.at(1), args.at(2));
    operacion.readWrite();
}
