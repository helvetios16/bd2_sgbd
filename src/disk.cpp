#include "../include/disk.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

Disk::Disk() {
    this->memory = 85899934592 / 2;       // 8gb
    this->platters = 4;                   // 2gb per platter
    this->surfaces = this->platters * 2;  // 1g per surface
    this->tracks = 20;                    // 64kb per track
    this->sectors = 4;                    // 640 bytes per sector
    this->blocks = 5;                     // 1mb per block
    this->memoryPerBlock = 1024 * 1024;
    this->memoryPerSector = 640;
}

void Disk::create(int memory, int platter, int track, int sector) {
    std::string path = "disk", platters = "platter", surfaces = "surface", tracks = "track", sectors = "sector";
    int sizeMemory = memory == -1 ? this->memory : memory;
    int sizePlatter = platter == -1 ? this->platters : platter;
    int sizeTrack = track == -1 ? this->tracks : track;
    int sizeSector = sector == -1 ? this->sectors : sector;
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
                                    for (int l = 0; l < sizeSector; l++) {
                                        std::string sector = track + "/" + sectors + " " + std::to_string(l);
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