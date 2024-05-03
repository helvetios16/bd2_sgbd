#include <iostream>

#include "include/disk.h"
#include "include/sgbd.h"
#include "include/shell.h"

int main(int argc, char const *argv[]) {
    // SGBD sgbd;
    // sgbd.shell();
    // Disk disk;
    // disk.getMemory();
    // disk.create();
    // disk.remove();
    Shell shell;
    shell.shell();
    return 0;
}
