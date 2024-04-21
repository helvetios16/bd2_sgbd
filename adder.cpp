#include "adder.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void Adder::addScheme(std::string archive) {
    std::fstream scheme("scheme.txt", std::ios::in | std::ios::out);
    if (!scheme.is_open()) {
        std::cout << "Error al abrir el archivo de esquemas" << std::endl;
        return;
    }
    std::string lineScheme;
    while (std::getline(scheme, lineScheme)) {
        std::istringstream ss(lineScheme);
        std::string firstWord;
        if (std::getline(ss, firstWord, '#') && firstWord == archive) {
            std::cout << "El nombre de la tabla ya esta registrado en el esquema" << std::endl;
            scheme.close();
            return;
        }
    }
    scheme.seekg(0, std::ios::end);
    scheme << archive << std::endl;
    scheme.close();
}