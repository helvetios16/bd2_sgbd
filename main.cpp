#include <iostream>

#include "include/sgbd.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    // sgbd.shell();
    sgbd.createTable("mundo");
    sgbd.addColumn("nombre string,edad int");

    return 0;
}
