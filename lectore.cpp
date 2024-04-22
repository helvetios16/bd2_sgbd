#include "lectore.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void Lectore::readCsv(const std::string& csv, const std::string& variable) {
    // separar el punto csv
    size_t pos = csv.rfind('.');
    std::string archive;
    if (pos != std::string::npos) {
        archive = csv.substr(0, pos);
    }
    // comprovar que no este en el equema es csv
    std::string searchLine = searchSheme(archive);
    if (archive == "") {
        std::cout << "El archivo csv ya se ha leido" << std::endl;
        return;
    }
    // pasar los datos de las columas al esquema
    std::fstream archiveCsv(csv, std::ios::in);
    if (!archiveCsv.is_open()) {
        std::cout << "Error al abrir el archivo csv" << std::endl;
        return;
    }
    std::string firstLine;
    std::getline(archiveCsv, firstLine);
    int sizeCsv = andi.sizeString(firstLine, ',');
    int sizeVarible = andi.sizeString(variable, ',');
    if (sizeCsv != sizeVarible) {
        std::cout << "Ingrese la cantidad correcta de tipo de variables respecto al csv" << std::endl;
        archiveCsv.close();
        return;
    }
    std::istringstream ssf(variable);
    std::string word;
    while (std::getline(ssf, word, ',')) {
        if (word == "int" || word == "float" || word == "string" || word == "bool" || word == "char") {
            continue;
        } else {
            std::cout << "Ingrese un dato de tipos correcto (string,int,float,char,bool)" << std::endl;
            archiveCsv.close();
            return;
        }
    }
    std::fstream scheme("scheme.txt", std::ios::out | std::ios::app);
    if (!scheme.is_open()) {
        std::cout << "Error al abrir el archivo de esquemas" << std::endl;
        return;
    }
    std::istringstream ssi(firstLine);
    std::istringstream ssv(variable);
    std::string line;
    std::string wordVarible;
    while (std::getline(ssi, word, ',')) {
        std::getline(ssv, wordVarible, ',');
        line += "#" + word + "#" + wordVarible;
    }
    scheme << archive << line << std::endl;
    scheme.close();
    // pasar los datos del csv a su archivo txt
    std::string namearchive = archive + ".txt";
    std::fstream archiveTable(namearchive, std::ios::out | std::ios::app);
    if (!archiveTable.is_open()) {
        std::cout << "Error en abrir el archivo de la tabla" << std::endl;
        return;
    }
    while (std::getline(archiveCsv, line)) {
        archiveTable << line << std::endl;
    }
    archiveTable.close();
}

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

// void Lectore::see(const std::string& archive, const std::string& columns, const std::string& condition, std::string& toPass);
