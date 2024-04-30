#include <iostream>

#include "include/sgbd.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    // sgbd.shell();
    // sgbd.see("edades", "*", "", "");
    sgbd.useDatabase("scheme");
    // sgbd.addCsvToTable("Titanic.csv", "Titanic");
    sgbd.showtable("otro_titanic");
    sgbd.see("otro_titanic", "PassengerId,Name", "", "");
    // sgbd.createTable("Hola");
    // sgbd.addColumn("nombre string", "Hola");
    // sgbd.createDatabase("scheme");
    // sgbd.useDatabase("scheme");
    return 0;
}
