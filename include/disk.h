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
    double memoryPerBlock;
    int sectors;
    int sectorPerBlock;
    double memoryPerSector;

   public:
    Disk();
    void create(int platters, int tracks, int sectors, int blocks);
    void setDisk();
    void createDefault();
    void remove();
    void checkInformation();
    void about();
    long long getMemory();
    void byDefault();
    double getMemoryPerSector();
    int getSectorPerBlock();
};