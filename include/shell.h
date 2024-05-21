#pragma once

#include <string>
#include <vector>

#include "disk.h"
#include "sgbd.h"

class Shell {
   private:
    SGBD sgbd;
    Disk disk;
    Memory memo;

   public:
    void shell();
    void sgbdControl(std::vector<std::string> tokens);
    bool haveSymbol(const std::string& str, const char& symbol);
};