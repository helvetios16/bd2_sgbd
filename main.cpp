#include <iostream>

#include "include/disk.h"
#include "include/sgbd.h"
#include "include/shell.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    sgbd.useDatabase("schema");
    // sgbd.addColumn("nombre string 64 ,edad int ,altura float", "prueba");
    // sgbd.showtable("prueba");
    // std::cout << sgbd.sizeString("prueba#nombre#string#64#edad#int#8#altura#float#8#year#int#8", '#') << std::endl;
    // sgbd.addCsvToTable("Titanic.csv", "_tng");
    // sgbd.addRegister("prueba", "Juan, 20, 1.70");
    // sgbd.addCsvToTable("Titanic.csv", "prueba_titanic");
    sgbd.see("prueba_titanic", "*", "", "inteno_1");
    // Disk disk;
    // disk.getMemory();
    // disk.create();
    // disk.remove();
    // Shell shell;
    // shell.shell();
    return 0;
}
