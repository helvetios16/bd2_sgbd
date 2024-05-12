#include "../include/memory.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <string>

void Memory::getBlocksToStack(const std::filesystem::path& path) {
    std::regex blockFileRegex("block [0-9]+\\.txt");  // Regex para archivos de bloques
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_regular_file(entry)) {
            std::string name = entry.path().filename().string();
            if (std::regex_search(name, blockFileRegex)) {
                momentBlock.push_back(entry.path().string());
            }
        } else if (std::filesystem::is_directory(entry)) {
            getBlocksToStack(entry.path());
        }
    }
}

void Memory::printVectorInArchive() {
    std::fstream file("out/memory.txt", std::ios::out);
    for (const auto& block : momentBlock) {
        file << block << std::endl;
    }
    file.close();
    momentBlock.clear();
}

std::string Memory::getDatabaseOfBlock(const std::string& relations) {  // posiblemente cambiar esto
    std::string archiveLine;
    std::fstream file("out/memory.txt", std::ios::in);
    std::string lineBlock, lineSector, pathOfSector;
    while (std::getline(file, lineBlock)) {
        std::fstream fileSector(lineBlock, std::ios::in);
        while (std::getline(fileSector, pathOfSector)) {
            for (int i = 0; i < 3; i++) std::getline(fileSector, lineSector);
            size_t found = lineSector.find(relations);
            if (found != std::string::npos) {
                archiveLine += pathOfSector + "\n";
            }
        }
    }
    return archiveLine;
}

std::string Memory::getRelationOfBlock(const std::string& relations) {  // cambiar esto
    std::string archiveLine;
    std::fstream file("out/memory.txt", std::ios::in);
    std::string lineBlock, lineSector, pathOfSector;
    while (std::getline(file, lineBlock)) {
        std::fstream fileSector(lineBlock, std::ios::in);
        while (std::getline(fileSector, pathOfSector)) {
            for (int i = 0; i < 2; i++) std::getline(fileSector, lineSector);
            size_t found = lineSector.find(relations);
            if (found != std::string::npos) {
                archiveLine += pathOfSector + "\n";
            }
            std::getline(fileSector, lineSector);
        }
    }
    return archiveLine;
}

void Memory::addInBlockRelation(const std::string& database, const std::string& relations) {
    std::fstream file("out/memory.txt", std::ios::in);
    std::fstream temp("out/temp.txt", std::ios::out);
    if (!file.is_open() || !temp.is_open()) {
        std::cout << "Error al abrir el archivo" << std::endl;
        return;
    }
    std::string line, path, memory, relation, dataBase, truePath;
    bool proof = false;
    bool yeah = true;
    while (std::getline(file, line)) {
        std::fstream fileBlock(line, std::ios::in);
        if (!fileBlock.is_open()) {
            std::cout << "Error al abrir el archivo" << std::endl;
            return;
        }
        while (std::getline(fileBlock, path)) {
            std::getline(fileBlock, memory);
            std::getline(fileBlock, relation);
            std::getline(fileBlock, dataBase);
            if (searchWordInLine(relation, database + "@" + relations)) {
                std::cout << "Ya existe la relacion" << std::endl;
                proof = true;
                break;
            }

            if (yeah) {
                if (disk.getMemoryPerSector() - std::stoi(memory) > 1000) {
                    relation += database + "@" + relations + "|";
                    truePath = path;
                    proof = true;
                    yeah = false;
                    memory = std::to_string(std::stoi(memory) + database.size() + relations.size() + 1);
                }
            }
            temp << path + "\n" + memory + "\n" + relation + "\n" + database + "\n";
        }
        fileBlock.close();
        if (proof || !yeah) break;
    }
    file.close();
    temp.close();
    if (proof && yeah) {
        std::cout << "No se ha agregado la relacion" << std::endl;
        return;
    }
    if (std::remove(line.c_str()) != 0) {
        std::cout << "Error al borrar el archivo" << std::endl;
        return;
    }
    if (std::rename("out/temp.txt", line.c_str()) != 0) {
        std::cout << "Error al renombrar el archivo" << std::endl;
        return;
    }
    std::fstream fileSector(truePath, std::ios::out | std::ios::app);
    if (!fileSector.is_open()) {
        std::cout << "Error al abrir el archivo 1" << std::endl;
        return;
    }
    fileSector << database + "Ø" + relations + "\n";
    fileSector.close();
    std::cout << "Se ha agregado la relacion" << std::endl;
}

bool Memory::searchWordInLine(const std::string& line, const std::string& word) {
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, '|')) {
        if (token == word) {
            return true;
        }
    }
    return false;
}

void Memory::addInBlockDatabase(const std::string& database) {
    std::fstream file("out/memory.txt", std::ios::in);
    std::fstream temp("out/temp.txt", std::ios::out);
    if (!file.is_open() || !temp.is_open()) {
        std::cout << "Error al abrir el archivo" << std::endl;
        return;
    }
    std::string line, path, memory, relation, dataBase, truePath;
    bool proof = false;
    bool yeah = true;
    while (std::getline(file, line)) {
        std::fstream fileBlock(line, std::ios::in);
        if (!fileBlock.is_open()) {
            std::cout << "Error al abrir el archivo" << std::endl;
            return;
        }
        while (std::getline(fileBlock, path)) {
            std::getline(fileBlock, memory);
            std::getline(fileBlock, relation);
            std::getline(fileBlock, dataBase);
            if (searchWordInLine(dataBase, database)) {
                std::cout << "Ya existe la base de datos" << std::endl;
                proof = true;
                break;
            }
            if (yeah) {
                if (disk.getMemoryPerSector() - std::stoi(memory) > 1000) {
                    dataBase += database + "|";
                    truePath = path;
                    proof = true;
                    yeah = false;
                }
            }
            temp << path + "\n" + memory + "\n" + relation + "\n" + dataBase + "\n";
        }
        fileBlock.close();
        if (proof || !yeah) break;
    }
    file.close();
    temp.close();
    if (proof && yeah) {
        std::cout << "No se ha agregado la base de datos" << std::endl;
        return;
    }
    if (std::remove(line.c_str()) != 0) {
        std::cout << "Error al borrar el archivo" << std::endl;
        return;
    }
    if (std::rename("out/temp.txt", line.c_str()) != 0) {
        std::cout << "Error al renombrar el archivo" << std::endl;
        return;
    }
}

void Memory::addRegisterInSectors(const std::string& database, const std::string& relation, const std::string& archive) {
    std::fstream file("out/memory.txt", std::ios::in);
    std::fstream archiveFile(archive, std::ios::in);
    std::string line, path, memory, relations, dataBase, truePath, linePath;
    int index = 0;
    // leer el archivo donde estan los bloques
    while (std::getline(file, line)) {
        std::fstream fileBlock(line, std::ios::in);
        // entrar a cada bloque y leer el archivo
        std::fstream filesSector("out/temp.txt", std::ios::out);
        while (std::getline(fileBlock, path)) {
            std::getline(fileBlock, memory);
            std::getline(fileBlock, relations);
            std::getline(fileBlock, dataBase);
            // si la base de datos y la relacion coinciden con las que se pasan por parametro
            // --------------- falta ---------------
            // se procede a agregar el registro en el sector
            int memorySpace = std::stoi(memory);
            std::fstream fileSector(path, std::ios::out | std::ios::app);
            for (int i = 0; i < index; i++) std::getline(archiveFile, linePath);
            while (std::getline(archiveFile, linePath)) {
                if ((memorySpace + linePath.size()) > disk.getMemoryPerSector()) {
                    break;
                }
                memorySpace += linePath.size();
                fileSector << linePath + "\n";
                index++;
            }
            filesSector << path + "\n" + std::to_string(memorySpace) + "\n" + relations + "\n" + dataBase + "\n";
        }
        filesSector.close();
        fileBlock.close();
        if (std::remove(line.c_str()) != 0) {
            std::cout << "Error al borrar el archivo" << std::endl;
            return;
        }
        if (std::rename("out/temp.txt", line.c_str()) != 0) {
            std::cout << "Error al renombrar el archivo" << std::endl;
            return;
        }
    }
    // si el archivo tiene la relacion agregar el registro en tal sector
    // en caso de que el sector se encuentre lleno buscar el siguiente sector y agregar el registro
    // si es necesario tiene que buscar un bloque para un sector
}
