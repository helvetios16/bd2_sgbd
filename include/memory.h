#pragma once

#include <filesystem>
#include <stack>
#include <string>
#include <vector>

#include "disk.h"

class Memory {
   private:
    // std::stack<std::string> memory;  // puedo usar stack o vector
    std::vector<std::string> momentBlock;
    int maxSizeOfVector = 8;  // convertir macro luego
    Disk disk;

   public:
    void getBlocksToStack(const std::filesystem::path& path);  // llamar solo una vez luego haver creado el disco
    void printVectorInArchive();                               // llamar solo una vez luego haver creado el disco
    void insideBlock(const std::filesystem::path& path);
    std::string getDatabaseOfBlock(const std::string& database);
    std::string getRelationOfBlock(const std::string& relations);
    void addInBlockRelation(const std::string& database, const std::string& relations);
};