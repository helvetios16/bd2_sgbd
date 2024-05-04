#include <iostream>

#include "include/disk.h"
#include "include/sgbd.h"
#include "include/shell.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    sgbd.createDatabase("schema");
    sgbd.useDatabase("schema");
    sgbd.createTable("prueba");
    sgbd.addColumn("prueba", "nombre string 64, edad int, altura float");
    sgbd.showtable("prueba");
    // Disk disk;
    // disk.getMemory();
    // disk.create();
    // disk.remove();
    // Shell shell;
    // shell.shell();
    return 0;
}
