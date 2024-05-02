#pragma once
#include <string>
class Disk {
   private:
    long long memory;
    int platters;
    int surfaces;
    int tracks;
    int blocks;
    int sectors;

   public:
    Disk();
    void createDisk();
    void about();
};