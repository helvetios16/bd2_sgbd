#include <iostream>

#include "include/sgbd.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    sgbd.see("Titanic", "Name,Age", "", "prueba");
    return 0;
}
