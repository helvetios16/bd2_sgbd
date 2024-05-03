#pragma once
#include <cstdint>
#include <string>
class Disk {
   private:
    uint64_t memory;  // long long
    int platters;
    int surfaces;
    int tracks;
    int blocks;
    int sectors;

   public:
    Disk();
    void create();
    void remove();
    void about();
};