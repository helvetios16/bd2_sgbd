#pragma once
#include <fstream>
#include <iostream>
#include <string>
class Disk {
   private:
    long long memory;
    int platters;
    int surfaces;
    int tracks;
    int blocks;
    int sectors;
    double memoryPerBlock;
    int sectorPerBlock;
    double memoryPerSector;

   public:
    Disk();
    void create(int platters, int tracks, int sectors, int blocks);
    void setDisk();
    void setNoDefault();
    void createDefault();
    void remove();
    void checkInformation();
    void about();
    long long getMemory();
    void byDefault();
    double getMemoryPerSector();
    int getSectorPerBlock();
    int getSector();
    int getBlock();
};