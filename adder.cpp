#include "adder.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void Adder::addScheme(const std::string& archive) {
    std::fstream scheme("scheme.txt", std::ios::in | std::ios::app | std::ios::out);
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
    scheme.clear();
    scheme << archive << std::endl;
    if (scheme.fail()) {
        std::cout << "Error en editar el archivo" << std::endl;
    }
    scheme.close();
}