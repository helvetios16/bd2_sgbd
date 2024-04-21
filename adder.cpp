#include "adder.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void Adder::addScheme(const std::string& archive, const std::string& variable) {
    std::fstream scheme("scheme.txt", std::ios::in | std::ios::out | std::ios::app);
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
    if (variable == "") {
        std::cout << "Ingrese la relacion(es) \"variable,tipo\" que tendra la tabla" << std::endl;
        scheme.close();
        return;
    }
    std::istringstream ssv(variable);
    std::string word;
    std::string lineNameVariable;
    int index = 1;
    while (std::getline(ssv, word, ',')) {
        if (index % 2 == 0 && !validType(word)) {
            std::cout << "Ingrese correctamente la relacion(es) \"variable,tipo\"" << std::endl;
            scheme.close();
            return;
        }
        index++;
        lineNameVariable += "#" + word;
    }
    scheme.clear();
    scheme << archive << lineNameVariable << std::endl;
    if (scheme.fail()) {
        std::cout << "Error en editar el archivo" << std::endl;
    }
    scheme.close();
}

bool Adder::validType(const std::string& type) {
    return (type == "string" || type == "int" || type == "float" || type == "char");
}