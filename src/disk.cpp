#include "../include/disk.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

Disk::Disk() {
    this->memory = 1073741824 / 2;  // 500mb;
    this->platters = 4;
    this->surfaces = 2;
    this->tracks = 8;
    this->blocks = 2;  // blocks per track
    this->memoryPerBlock = 1048576 * 4;
    this->sectors = 8;  // sectors per track
    this->sectorPerBlock = 4;
    this->memoryPerSector = 1048576;
}

void Disk::create(int platter, int track, int sector, int blocks) {
    std::string path = "disk", platters = "platter", surfaces = "surface", tracks = "track", sectors = "sector", blcks = "block";
    int sizePlatter = platter;
    int sizeTrack = track;
    int sizeSector = sector;
    int sizeBlock = blocks;
    if (!std::filesystem::exists(path)) {
        if (std::filesystem::create_directory(path)) {
            for (int i = 0; i < sizePlatter; i++) {
                std::string platter = path + "/" + platters + " " + std::to_string(i + 1);
                if (std::filesystem::create_directory(platter)) {
                    for (int j = 0; j < 2; j++) {  // 2 surfaces per platter
                        std::string surface = platter + "/" + surfaces + " " + std::to_string(j + 1);
                        if (std::filesystem::create_directory(surface)) {
                            for (int k = 0; k < sizeTrack; k++) {
                                std::string track = surface + "/" + tracks + " " + std::to_string(k + 1);
                                if (std::filesystem::create_directory(track)) {
                                    for (int m = 0; m < sizeBlock; m++) {
                                        std::string block = track + "/" + blcks + " " + std::to_string(m + 1);
                                        if (std::filesystem::create_directory(block)) {
                                            for (int l = 0; l < sizeSector / sizeBlock; l++) {
                                                std::string sector = block + "/" + sectors + " " + std::to_string(l + 1);
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
    create(this->platters, this->tracks, this->sectors, this->blocks);
}

void Disk::setDisk() {
    std::cout << "Ingrese la cantidad de platos: ";
    std::cin >> this->platters;
    std::cout << "Ingrese la cantidad de superficies: ";
    std::cin >> this->surfaces;
    std::cout << "Ingrese la cantidad de pistas: ";
    std::cin >> this->tracks;
    std::cout << "Ingrese la cantidad de bloques por pista: ";
    std::cin >> this->blocks;
    std::cout << "Ingrese la cantidad de sectores por pista: ";
    std::cin >> this->sectors;
    std::cout << "Ingrese la cantidad de sectores por bloque: ";
    std::cin >> this->sectorPerBlock;
    std::cout << "Ingrese la cantidad de memoria por bloque: ";
    std::cin >> this->memoryPerBlock;
    std::cout << "Ingrese la cantidad de memoria por sector: ";
    std::cin >> this->memoryPerSector;
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

void Disk::about() {
    std::cout << "Informacion del disco" << std::endl;
    std::cout << "Memoria: " << this->memory << std::endl;
    std::cout << "Platos: " << this->platters << std::endl;
    std::cout << "Superficies: " << this->surfaces << std::endl;
    std::cout << "Pistas: " << this->tracks << std::endl;
    std::cout << "Bloques por pista: " << this->blocks << std::endl;
    std::cout << "Memoria por bloque: " << this->memoryPerBlock << std::endl;
    std::cout << "Sectores por pista: " << this->sectors << std::endl;
    std::cout << "Sectores por bloque: " << this->sectorPerBlock << std::endl;
    std::cout << "Memoria por sector: " << this->memoryPerSector << std::endl;
}

void Disk::checkInformation() {
    if (this->sectors % this->blocks != 0) {
        std::cout << "Los bloques tiene que ser multiplos de la cantidad de sectores por bloque" << std::endl;
        return;
    }
    if ((this->memory) / (this->platters * this->surfaces * this->tracks * this->sectors) != (this->memoryPerSector)) {
        std::cout << "La cantidad de memoria no coincide con la cantidad de memoria por el total de sectores" << std::endl;
        return;
    }
    if (this->memoryPerBlock != this->memoryPerSector * this->sectorPerBlock) {
        std::cout << "La cantidad de memoria por bloque no coincide con la cantidad de memoria por sector" << std::endl;
        return;
    }
    std::cout << "Todo correcto" << std::endl;
}

long long Disk::getMemory() { return this->memory; }

void Disk::byDefault() {
    this->memory = 1073741824 / 2;  // 500mb;
    this->platters = 4;
    this->surfaces = 2;
    this->tracks = 8;
    this->blocks = 2;  // blocks per track
    this->memoryPerBlock = 1048576 * 4;
    this->sectors = 8;  // sectors per track
    this->sectorPerBlock = 4;
    this->memoryPerSector = 1048576;
}

// luego guardar en la info en un txt para mantener la informacion si desea configurar el disco logicamente que este en otro lado del disco

double Disk::getMemoryPerSector() { return this->memoryPerSector; }

int Disk::getSectorPerBlock() { return this->sectorPerBlock; }