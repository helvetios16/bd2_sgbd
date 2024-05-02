#include "../include/disk.h"

#include <iomanip>
#include <iostream>

Disk::Disk() {
    this->memory = 85899934592;           // 8gb
    this->platters = 4;                   // 2gb per platter
    this->surfaces = this->platters * 2;  // 1g per surface
    this->tracks = 16384;                 // 64kb per track
    this->sectors = 1024;                 // 640 bytes per sector
}

void Disk::createDisk() {
    std::cout << "Creating disk..." << std::endl;
    std::cout << std::setw(16) << std::left << "Memory:" << std::setw(10) << this->memory << " bytes" << std::endl;
    std::cout << std::setw(16) << std::left << "Platters:" << std::setw(10) << this->platters << std::endl;
    std::cout << std::setw(16) << std::left << "Memory Platters:" << std::setw(10) << this->memory / this->platters << " bytes" << std::endl;
    std::cout << std::setw(16) << std::left << "Surfaces:" << std::setw(10) << this->surfaces << std::endl;
    std::cout << std::setw(16) << std::left << "Memory Surfaces:" << std::setw(10) << this->memory / this->surfaces << " bytes" << std::endl;
    std::cout << std::setw(16) << std::left << "Tracks:" << std::setw(10) << this->tracks << std::endl;
    std::cout << std::setw(16) << std::left << "Memory Tracks:" << std::setw(10) << (this->memory / this->surfaces) / this->tracks << " bytes" << std::endl;
    std::cout << std::setw(16) << std::left << "Sectors:" << std::setw(10) << this->sectors << std::endl;
    std::cout << std::setw(16) << std::left << "Memory Sectors:" << std::setw(10) << ((this->memory / this->surfaces) / this->tracks) / this->sectors << " bytes" << std::endl;
}