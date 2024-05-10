#pragma once
#include <iostream>
#include <string>
class Disk {
   private:
    long long memory;
    int platters;
    int surfaces;
    int tracks;
    int blocks;
    int memoryPerBlock;
    int sectors;
    int sectorPerBlock;
    int memoryPerSector;

   public:
    Disk();
    void create(int memory, int platters, int tracks, int sectors, int blocks);
    void createDefault();
    void remove();
    void about();
    void checkInformation();
};