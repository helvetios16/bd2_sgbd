#pragma once
#include "disk.h"
#include "sgbd.h"
class Shell {
   private:
    SGBD sgbd;
    Disk disk;
};