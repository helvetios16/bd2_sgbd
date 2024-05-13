#include <iostream>

#include "include/disk.h"
#include "include/memory.h"
#include "include/sgbd.h"
#include "include/shell.h"

int main(int argc, char const *argv[]) {
    Disk disk;
    // disk.remove();
    // disk.createDefault();
    // disk.getMemory();
    // disk.createDefault();
    // disk.checkInformation();
    // disk.about();
    // Shell shell;
    // shell.shell();
    Memory mery;
    // mery.addInBlockRelation("db-schema", "movie");
    // mery.addInBlockDatabase("db-scheme");
    mery.addRegisterInSectors("db-schema", "titanic", "out/register.txt", 50);
    // mery.addInBlockTable();
    // mery.getBlocksToStack("disk"); // llamar solo una vez luego haver creado el disco
    // mery.printVectorInArchive(); // llamar solo una vez luego haver creado el disco
    return 0;
}
