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
        // if (numberGuide == maxSizeOfVector) {
        //     break;
        // }
        if (std::filesystem::is_regular_file(entry)) {
            std::string name = entry.path().filename().string();
            if (std::regex_search(name, blockFileRegex)) {
                momentBlock.push_back(entry.path().string());
                numberGuide++;
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
}