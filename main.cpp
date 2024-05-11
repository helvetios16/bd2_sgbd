#include <iostream>

#include "include/disk.h"
#include "include/memory.h"
#include "include/sgbd.h"
#include "include/shell.h"

int main(int argc, char const *argv[]) {
    Disk disk;
    disk.remove();
    disk.createDefault();
    // disk.getMemory();
    // disk.createDefault();
    // disk.checkInformation();
    // disk.about();
    // Shell shell;
    // shell.shell();
    // Memory memo;
    // memo.getBlocksToStack("disk");
    // memo.searchArchiveInBlocks("hola");
    return 0;
}
