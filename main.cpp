#include <iostream>

#include "include/sgbd.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    sgbd.createDatabase("esquema");
    sgbd.useDatabase("esquema");
    sgbd.createTable("edades");
    // sgbd.addColumn("nombre string", "edades");
    sgbd.showtable("edades");
    // sgbd.shell();
    // sgbd.see("edades", "*", "", "");
    // sgbd.addCsvToTable("Titanic.csv", "Titanic");
    // sgbd.showtable("otro_titanic");
    // sgbd.see("otro_titanic", "PassengerId,Name", "", "");
    // sgbd.createTable("Hola");
    // sgbd.addColumn("nombre string", "Hola");
    // sgbd.useDatabase("scheme");
    return 0;
}
