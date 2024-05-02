#include <iostream>

#include "include/disk.h"
#include "include/sgbd.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    sgbd.shell();
    // sgbd.useDatabase("scheme");
    // sgbd.see("netflix", "*", "", "");
    // Disk disk;
    // disk.createDisk();
    return 0;
}
