#include "../include/disk.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

Disk::Disk() {
    this->memory = 85899934592 / 2;  // 8gb
    this->platters = 2;              // 2gb per platter
    this->surfaces = 2;              // 1g per surface
    this->tracks = 5;                // 64kb per track
    this->sectors = 8;               // 640 bytes per sector
    this->blocks = 2;                // 1mb per block
    this->memoryPerBlock = 1024 * 1024;
    this->memoryPerSector = 640;
}

void Disk::create(int memory, int platter, int track, int sector, int blocks) {
    std::string path = "disk", platters = "platter", surfaces = "surface", tracks = "track", sectors = "sector", blcks = "block";
    int sizeMemory = memory;
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