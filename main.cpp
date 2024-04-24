#include <iostream>

#include "include/sgbd.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    sgbd.see("Titanic", "*", "Sex == 20", "");
    return 0;
}
