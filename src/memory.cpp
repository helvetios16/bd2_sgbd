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

std::vector<std::string> Memory::getVectorBlocks() { return momentBlock; }

void Memory::searchArchive(const std::string& archive) {
    searchArchiveInBlocks(archive);
    searchArchiveGeneral(archive);
}

void Memory::searchArchiveInBlocks(const std::string& archive) {
    std::string temp;
    bool proof = false;
    for (const auto& block : momentBlock) {
        std::filesystem::path pathArchive(block);
        temp = searchArchivePartial(archive + ".txt", pathArchive);
        proof = temp != "";
        if (proof) break;
    }
    proof ? std::cout << temp << std::endl : std::cout << "Archive not found" << std::endl;
}

void Memory::searchArchiveGeneral(const std::string& archive) {
    std::string temp;
    bool proof = false;
    std::filesystem::path path = "disk";
    temp = searchArchivePartial(archive, path);
    proof = temp != "";
    proof ? std::cout << temp << std::endl : std::cout << "Archive not found" << std::endl;
}

std::string Memory::searchArchivePartial(const std::string& archive, const std::filesystem::path& path) {
    for (const auto& entrance : std::filesystem::directory_iterator(path)) {
        if (entrance.is_regular_file() && entrance.path().filename() == archive) {
            return entrance.path().string();
        } else if (entrance.is_directory()) {
            std::string result = searchArchivePartial(archive, entrance.path());
            if (!result.empty()) return result;
        }
    }
    return "";
}