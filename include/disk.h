#pragma once
#include <string>
#include <vector>
class Disk {
   private:
    std::vector<std::string> files;
    int saucer;
    int block;
    int size;

   public:
    Disk();
    void about();
};