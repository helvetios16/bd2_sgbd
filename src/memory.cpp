#include "../include/memory.h"

#include <filesystem>
#include <iostream>
#include <regex>
#include <stack>
#include <string>

void Memory::getBlocksToStack(const std::filesystem::path& path) {
    std::regex blockRegex("block [0-9]+");
    for (const auto& entrance : std::filesystem::directory_iterator(path)) {
        if (numberGuide == maxSizeOfStack) {
            break;
        }
        if (std::filesystem::is_directory(entrance)) {
            std::string name = entrance.path().filename().string();
            if (std::regex_search(name, blockRegex)) {
                memory.push(entrance.path().string());
                numberGuide++;
            } else {
                getBlocksToStack(entrance.path());
            }
        }
    }
}

void Memory::printStack() {
}