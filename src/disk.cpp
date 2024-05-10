#include "../include/disk.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

Disk::Disk() {
    this->memory = 42949967296 / 8;  // 500mb;
    this->platters = 4;
    this->surfaces = 2;
    this->tracks = 8;
    this->blocks = 2;  // blocks per track
    this->memoryPerBlock = 2621457 * 4;
    this->sectors = 8;  // sectors per track
    this->sectorPerBlock = 4;
    this->memoryPerSector = 2621457;
}

void Disk::create(int memory, int platter, int track, int sector, int blocks) {
    std::string path = "disk", platters = "platter", surfaces = "surface", tracks = "track", sectors = "sector", blcks = "block";
    long long sizeMemory = memory;
    int sizePlatter = platter;
    int sizeTrack = track;
    int sizeSector = sector;
    int sizeBlock = blocks;
    if (!std::filesystem::exists(path)) {
        if (std::filesystem::create_directory(path)) {
            for (int i = 0; i < sizePlatter; i++) {
                std::string platter = path + "/" + platters + " " + std::to_string(i);
                if (std::filesystem::create_directory(platter)) {
                    for (int j = 0; j < 2; j++) {  // 2 surfaces per platter
                        std::string surface = platter + "/" + surfaces + " " + std::to_string(j);
                        if (std::filesystem::create_directory(surface)) {
                            for (int k = 0; k < sizeTrack; k++) {
                                std::string track = surface + "/" + tracks + " " + std::to_string(k);
                                if (std::filesystem::create_directory(track)) {
                                    for (int m = 0; m < sizeBlock; m++) {
                                        std::string block = track + "/" + blcks + " " + std::to_string(m);
                                        if (std::filesystem::create_directory(block)) {
                                            for (int l = 0; l < sizeSector / sizeBlock; l++) {
                                                std::string sector = block + "/" + sectors + " " + std::to_string(l);
                                                if (std::filesystem::create_directory(sector)) {
                                                    continue;
                                                } else {
                                                    std::cout << "Error creating " << sector << std::endl;
                                                }
                                            }
                                        } else {
                                            std::cout << "Error creating " << track << std::endl;
                                        }
                                    }
                                } else {
                                    std::cout << "Error creating " << track << std::endl;
                                }
                            }
                        } else {
                            std::cout << "Error creating " << surface << std::endl;
                        }
                    }
                } else {
                    std::cout << "Error creating " << platter << std::endl;
                }
            }
            return;
        } else {
            std::cout << "Error creating " << path << std::endl;
        }
    }
}

void Disk::createDefault() {
    create(this->memory, this->platters, this->tracks, this->sectors, this->blocks);
}

void Disk::remove() {
    std::string path = "disk";
    if (std::filesystem::exists(path)) {
        if (std::filesystem::remove_all(path)) {
            std::cout << "Disk removed" << std::endl;
            return;
        } else {
            std::cout << "Error removing " << path << std::endl;
        }
    } else {
        std::cout << "Disk does not exist" << std::endl;
    }
}

void Disk::checkInformation() {
    if (this->sectors % this->blocks != 0) {
        std::cout << "Los bloques tiene que ser multiplos de la cantidad de sectores por bloque" << std::endl;
        return;
    }
    if (this->memoryPerBlock != this->memoryPerSector * this->sectorPerBlock) {
        std::cout << "La cantidad de memoria por bloque no coincide con la cantidad de memoria por sector" << std::endl;
        return;
    }
    std::cout << "Todo correcto" << std::endl;
}