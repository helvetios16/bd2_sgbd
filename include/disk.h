#pragma once
#include <cstdint>
#include <string>
class Disk {
   private:
    uint64_t memory;
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