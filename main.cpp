#include <iostream>

#include "include/sgbd.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    // sgbd.shell();
    // sgbd.showtable("Titanic");
    // sgbd.addCsvToTable("Titanic.csv", "Titanic");
    sgbd.see("edades", "*", "", "");
    // sgbd.createDatabase("scheme");
    // sgbd.useDatabase("scheme");
    return 0;
}
