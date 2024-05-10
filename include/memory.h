#pragma once

#include <filesystem>
#include <stack>
#include <string>

class Memory {
   private:
    std::stack<std::string> memory;  // puedo usar stack o vector
    int maxSizeOfStack = 8;
    int numberGuide = 0;

   public:
    void getBlocksToStack(const std::filesystem::path& path);
    void printStack();
};