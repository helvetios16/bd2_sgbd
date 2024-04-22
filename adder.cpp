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
    // comprobar si el nombre de la tabla ya esta dentro del esquemas
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
    // verficar si la relacion para pasar la tabla conicnide con variable,tipo (...)
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
    // pasar la infomacion al esquema
    scheme.clear();
    scheme << archive << lineNameVariable << std::endl;
    if (scheme.fail()) {
        std::cout << "Error en editar el archivo" << std::endl;
    }
    scheme.close();
}

void Adder::addRegister(const std::string& archive, const std::string& variable) {
    std::fstream scheme("scheme.txt", std::ios::in);
    if (!scheme.is_open()) {
        std::cout << "Error al abrir el archivo de esquemas" << std::endl;
        return;
    }
    // en varible verificar que este entre () auqneu se puede dejar a la shell
    std::string lineScheme;
    std::string lineArchive;
    bool table = false;
    while (std::getline(scheme, lineScheme)) {
        std::istringstream ss(lineScheme);
        std::string firstWord;
        if (std::getline(ss, firstWord, '#') && firstWord == archive) {
            // validar que se encuentre el nombre de la tabla
            table = true;
            int sizeTable = sizeString(lineScheme, '#');
            int sizeVariable = sizeString(variable, ',');
            // comrpobar que la cantidad de datos a registrar coninciar en cierto punto la cantdidad de columnas de la tabla}
            if (!(sizeVariable == sizeTable)) {
                std::cout << "Ingrese una cantidad de registros acorde a la tabla" << std::endl;
                scheme.close();
                return;
            }
            std::string nextWord;
            std::string secondaryWord;
            std::istringstream ssv(variable);
            while (std::getline(ss, nextWord, '#')) {
                std::getline(ss, nextWord, '#');
                std::getline(ssv, secondaryWord, ',');
                // luego comprobar que si es un int o float o char o bool
                if (!checkType(nextWord, secondaryWord)) {
                    std::cout << "Ingrese el dato correcto al tipo de variable" << std::endl;
                    scheme.close();
                    return;
                }
            }
        }
        if (table) break;
    }
    if (!table) {
        std::cout << "No se encuentra la tabla en el esquema" << std::endl;
        return;
    }
    scheme.close();
    std::string nameArchive = archive + ".txt";
    std::fstream archiveTable(nameArchive, std::ios::out | std::ios::app);
    archiveTable << variable << std::endl;
    archiveTable.close();
    // pasar los datos al documento
}

bool Adder::validType(const std::string& type) {
    return (type == "string" || type == "int" || type == "float" || type == "char" || type == "bool");
}

int Adder::sizeString(const std::string& line, const char& symbol) {
    std::string temp = line;
    std::istringstream ss(temp);
    int index = 0;
    std::string word;
    while (std::getline(ss, word, symbol)) {
        index++;
    }
    if (symbol == '#') return (index / 2);
    return index;
}

bool Adder::checkType(const std::string& type, const std::string& variable) {
    if (type == "string") {
        // si la profesora dice que el string este entre comillas agregar estrutucracion
        return true;
    } else if (type == "int" || type == "float") {
        return convertToNumber(variable, type);
    } else if (type == "bool") {
        if (variable == "1" || variable == "0") {
            return true;
        }
    } else if (type == "char") {
        return variable.size() == 1;
    }
    return false;
}

bool Adder::convertToNumber(const std::string& number, const std::string& type) {
    try {
        size_t pos = 0;
        if (type == "int") {
            std::stoi(number, &pos);
        }
        if (type == "float") {
            std::stof(number, &pos);
        }
        return pos == number.size();
    } catch (const std::exception& e) {
        return false;
    }
}