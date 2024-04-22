#include "lectore.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// void readCsv(const std::string& csv, const std::string& variable);

std::string Lectore::searchSheme(const std::string& archive) {
    std::fstream scheme("scheme.txt", std::ios::in);
    if (!scheme.is_open()) {
        std::cout << "Error al abrir el archivo de esquemas" << std::endl;
        return "";
    }
    std::string lineSearch;
    while (std::getline(scheme, lineSearch)) {
        std::istringstream ss(lineSearch);
        std::string firstWord;
        if (std::getline(ss, firstWord, '#') && firstWord == archive) {
            return lineSearch;
        }
    }
    return "";
}

// void see(const std::string& archive, const std::string& columns, const std::string& condition, std::string& toPass);
