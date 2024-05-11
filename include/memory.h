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
    int numberGuide = 0;
    bool otherProof = false;
    Disk disk;

   public:
    void getBlocksToStack(const std::filesystem::path& path);  // llamar solo una vez luego haver creado el disco
    void printVectorInArchive();                               // llamar solo una vez luego haver creado el disco
    std::vector<std::string> getVectorBlocks();
    void searchArchive(const std::string& archive);  // cambiar las funciones de busqueda de void  a string
    void searchArchiveInBlocks(const std::string& archive);
    std::string searchArchivePartial(const std::string& archive, const std::filesystem::path& path);
    void searchArchiveGeneral(const std::string& archive);
    void startArchive(const std::string& archive);
    void createArchive(const std::string& archive);
    void verifySizeArchive(const std::string& archive);
    void moveArchive(const std::string& archive);
};