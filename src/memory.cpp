#include "../include/memory.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
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

void Memory::addInBlockTable() {
    std::fstream file("out/memory.txt", std::ios::in);
    std::string line;
    while (std::getline(file, line)) {
        insideBlock(line);
        break;
    }
}

void Memory::insideBlock(const std::filesystem::path& path) {
    std::fstream file(path, std::ios::in);
    std::string paths, memory, relations, databases;
    while (std::getline(file, paths)) {
        std::getline(file, memory);
        std::getline(file, relations);
        std::getline(file, databases);
        std::cout << paths << std::endl;
        std::cout << memory << std::endl;
        std::cout << relations << std::endl;
        std::cout << databases << std::endl;
        break;
    }
}