#include <iostream>

#include "sgbd.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    sgbd.see("Titanic", "*", "Age > 45", "");
    return 0;
}
