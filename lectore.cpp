#include "lectore.h"

#include <fstream>
#include <iomanip>
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
    if (searchLine == "") {
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

void Lectore::see(const std::string& archive, const std::string& columns, const std::string& condition, const std::string& toPass) {
    std::string searchLine = searchSheme(archive);
    if (searchLine == "") {
        std::cout << "El archivo no se encuentra resgistrado" << std::endl;
        return;
    }
    const int columnWidth = 16;
    int sizeArchive = andi.sizeString(searchLine, '#');
    std::string namearchive = archive + ".txt";
    std::fstream archiveTable(namearchive, std::ios::in);
    if (!archiveTable.is_open()) {
        std::cout << "Error en abrir el archivo de la tabla" << std::endl;
        return;
    }
    // Agregar comprobacion correcta de conditions al toPass y para la condicional
    std::string lineTable;
    std::string lineToColumn;
    std::string lineToData;
    if (columns == "*") {
        std::istringstream ssi(searchLine);
        std::string fisrt;
        std::getline(ssi, fisrt, '#');
        std::stringstream formString;
        while (std::getline(ssi, fisrt, '#')) {
            formString << std::setw(columnWidth) << std::left << fisrt.substr(0, columnWidth - 2);
            std::getline(ssi, fisrt, '#');
        }
        std::string stripes(columnWidth * sizeArchive, '-');
        std::cout << formString.str() << std::endl;
        std::cout << stripes << std::endl;

        while (std::getline(archiveTable, lineTable)) {
            std::istringstream ss(lineTable);
            std::string momentWord;
            std::stringstream formattedString;
            while (std::getline(ss, momentWord, ',')) {
                if (momentWord.size() >= 1 && momentWord.front() == '"' && momentWord.back() != '"') {
                    size_t nextCommaPos = lineTable.find(',', ss.tellg());
                    if (lineTable[nextCommaPos - 1] == '"') {
                        momentWord += lineTable.substr(ss.tellg(), nextCommaPos - ss.tellg());
                        ss.seekg(nextCommaPos + 1);
                    }
                }
                formattedString << std::setw(columnWidth) << std::left << momentWord.substr(0, columnWidth - 2);
            }
            std::cout << formattedString.str() << std::endl;
        }
    }
}
