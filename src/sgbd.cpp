#include "../include/sgbd.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/memory.h"

#define COLUMN_WIDTH 16

SGBD::SGBD() {
    std::fstream file("out/scheme.txt", std::ios::in);
    if (!file) {
        std::fstream scheme("out/scheme.txt", std::ios::out);
        if (!scheme) {
            std::cout << "Error al abrir el archivo de esquemas" << std::endl;
        } else {
            scheme.close();
            return;
        }
    } else {
        file.close();
    }
}

void SGBD::createDatabase(const std::string& db) {
    std::fstream file("out/" + db + ".txt", std::ios::in);
    if (file) {
        std::cout << "Esta base de datos ya ha sido creada" << std::endl;
        file.close();
        return;
    } else {
        std::fstream newDb("out/" + db + ".txt", std::ios::out);
        newDb.close();
    }
}

void SGBD::useDatabase(const std::string& db) {
    std::fstream file("out/" + db + ".txt", std::ios::in);
    if (file) {
        this->database = "out/" + db + ".txt";
    } else {
        std::cout << "No existe esta base de datos" << std::endl;
    }
    file.close();
}

void SGBD::createTable(const std::string& archive) {
    std::string file = searchSheme(archive);
    if (file == "") {
        std::fstream scheme(this->database, std::ios::out | std::ios::app);
        if (!scheme.is_open()) {
            std::cout << "Error al abrir el archivo de esquemas" << std::endl;
            return;
        }
        scheme << archive << std::endl;
        scheme.close();
    } else {
        std::cout << "La tabla ya esta registrado" << std::endl;
    }
}

void SGBD::addColumn(const std::string& information, const std::string& archive) {
    std::istringstream ss(information);
    std::string line, word, otherWord, passLine;
    while (std::getline(ss, line, ',')) {
        std::istringstream ssi(line);
        while (std::getline(ssi, word, ' ')) {
            std::getline(ssi, otherWord, ' ');
            if (!validType(otherWord)) {
                std::cout << "El tipo de dato no es valido" << std::endl;
                return;
            }
            if (otherWord == "int" || otherWord == "float") {
                passLine += "#" + word + "#" + otherWord + "#" + "8";
            } else if (otherWord == "string" || otherWord == "char") {
                passLine += "#" + word + "#" + otherWord;
                std::getline(ssi, otherWord, ' ');
                if (!isdigit(otherWord[0])) {
                    std::cout << "Se tiene que registrar un numero para las cadenas de texto " << std::endl;
                    return;
                } else {
                    passLine += "#" + otherWord;
                }
            } else if (otherWord == "bool") {
                passLine += "#" + word + "#" + otherWord + "#" + "1";
            }
        }
    }
    std::fstream scheme(this->database, std::ios::in | std::ios::out);
    std::fstream temp("out/temp.txt", std::ios::out);
    if (!scheme.is_open() || !temp.is_open()) {
        std::cout << "Error al abrir el archivo de esquemas" << std::endl;
        return;
    }
    while (std::getline(scheme, line)) {
        std::istringstream ss(line);
        if (std::getline(ss, word, '#')) {
            if (word == archive) {
                temp << line + passLine << std::endl;
            } else {
                temp << line << std::endl;
            }
        }
    }
    scheme.close();
    temp.close();
    std::remove(this->database.c_str());
    std::rename("out/temp.txt", this->database.c_str());
}

void SGBD::showtable(const std::string& archive) {
    std::string file = searchSheme(archive);
    if (file == "") {
        std::cout << "La tabla no se encuentra registrada" << std::endl;
        return;
    }
    std::string word;
    std::istringstream ss(file);
    std::stringstream formString;
    std::getline(ss, word, '#');
    std::cout << word << std::endl;
    std::string stripes(COLUMN_WIDTH * 3, '-');
    std::cout << stripes << std::endl;
    while (std::getline(ss, word, '#')) {
        formString << std::setw(COLUMN_WIDTH) << std::left << word.substr(0, COLUMN_WIDTH - 2);
        std::getline(ss, word, '#');
        formString << std::setw(COLUMN_WIDTH) << std::left << word.substr(0, COLUMN_WIDTH - 2);
        std::getline(ss, word, '#');
        formString << std::setw(COLUMN_WIDTH) << std::left << word.substr(0, COLUMN_WIDTH - 2) << std::endl;
    }
    std::cout << formString.str();
}

void SGBD::addCsvToTable(const std::string& csv, const std::string& archive) {
    std::string file = searchSheme(archive);
    if (file == "") {
        std::cout << "La tabla no se encuentra registrada" << std::endl;
        return;
    }
    std::fstream archiveTable("out/" + archive + ".txt", std::ios::out | std::ios::app);
    if (!archiveTable.is_open()) {
        std::cout << "Error al abrir el archivo de la tabla" << std::endl;
        return;
    }
    std::fstream archiveCsv("csv/" + csv, std::ios::in);
    if (!archiveCsv.is_open()) {
        std::cout << "Error al abrir el archivo csv" << std::endl;
        return;
    }
    std::string line, otherWord, word;
    std::getline(archiveCsv, line);
    int sizeFile = sizeString(file, '#');
    if (sizeFile != sizeString(line, ',')) {
        std::cout << "La cantidad de columnas no coincide con la tabla" << std::endl;
        archiveCsv.close();
        archiveTable.close();
        return;
    }
    std::stringstream formString;
    while (std::getline(archiveCsv, line)) {
        std::istringstream ss(file);
        std::istringstream sss(line);
        std::getline(ss, otherWord, '#');
        while (std::getline(ss, otherWord, '#')) {
            std::getline(ss, otherWord, '#');
            std::getline(ss, otherWord, '#');
            std::getline(sss, word, ',');
            bool endOfLine = false;
            if (word.size() >= 1 && word.front() == '"' && word.back() != '"') {
                size_t nextCommaPoss = line.find("\",", sss.tellg());
                if (nextCommaPoss != std::string::npos) {
                    word += "," + line.substr(sss.tellg(), nextCommaPoss - sss.tellg() + 1);
                    sss.seekg(nextCommaPoss + 2);
                } else {
                    word += "," + line.substr(ss.tellg());
                    endOfLine = true;
                }
            }
            formString << std::setw(std::stoi(otherWord)) << std::left << word;
            if (endOfLine) break;
        }
        formString << std::endl;
    }
    archiveTable << formString.str();
    archiveTable.close();
    archiveCsv.close();
}

void SGBD::addRegister(const std::string& archive, const std::string& variable) {
    std::fstream scheme(this->database, std::ios::in);
    if (!scheme.is_open()) {
        std::cout << "Error al abrir el archivo de esquemas" << std::endl;
        return;
    }
    std::string lineScheme, lineArchive, lineOfArchive;
    bool table = false;
    while (std::getline(scheme, lineScheme)) {
        std::istringstream ss(lineScheme);
        std::string firstWord;
        if (std::getline(ss, firstWord, '#') && firstWord == archive) {
            // validar que se encuentre el nombre de la tabla
            table = true;
            lineOfArchive = lineScheme;
            int sizeTable = sizeString(lineScheme, '#');
            int sizeVariable = sizeString(variable, ',');
            // comrpobar que la cantidad de datos a registrar coninciar en cierto punto la cantdidad de columnas de la tabla}
            if (!(sizeVariable == sizeTable)) {
                std::cout << "Ingrese una cantidad de registros acorde a la tabla" << std::endl;
                scheme.close();
                return;
            }
            std::string nextWord, secondaryWord;
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
                if (nextWord == "string") {
                    int sizeWord = secondaryWord.size();
                    std::getline(ss, nextWord, '#');
                    if (sizeWord > std::stoi(nextWord)) {
                        std::cout << "El tamaño de la cadena es mayor al permitido" << std::endl;
                        scheme.close();
                        return;
                    }
                    continue;
                }
                // la tercera palabras son numeros entonces lo pasamos
                std::getline(ss, nextWord, '#');
            }
        }
        if (table) break;
    }
    if (!table) {
        std::cout << "No se encuentra la tabla en el esquema" << std::endl;
        return;
    }
    scheme.close();
    std::fstream archiveTable("out/" + archive + ".txt", std::ios::out | std::ios::app);
    std::string input, otherInput;
    std::stringstream formString;
    std::istringstream ssv(lineOfArchive), svs(variable);
    std::getline(ssv, input, '#');
    while (std::getline(ssv, input, '#')) {
        std::getline(ssv, input, '#');
        std::getline(ssv, input, '#');
        std::getline(svs, otherInput, ',');
        formString << std::setw(std::stoi(input)) << std::left << otherInput;
    }
    archiveTable << formString.str() << std::endl;
    archiveTable.close();
}

bool SGBD::validType(const std::string& type) {
    return (type == "string" || type == "int" || type == "float" || type == "char" || type == "bool" || type == "date");
}

int SGBD::sizeString(const std::string& line, const char& symbol) {
    std::string temp = line;
    std::istringstream ss(temp);
    int index = 0;
    std::string word;
    while (std::getline(ss, word, symbol)) {
        index++;
    }
    if (symbol == '#') return (index / 3);
    return index;
}

bool SGBD::checkType(const std::string& type, const std::string& variable) {
    if (type == "string") {
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

bool SGBD::convertToNumber(const std::string& number, const std::string& type) {
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

std::string SGBD::searchSheme(const std::string& archive) {
    std::fstream scheme(this->database, std::ios::in);
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

void SGBD::see(const std::string& archive, const std::string& columns, const std::string& condition, const std::string& toPass) {
    this->searchLine = searchSheme(archive);
    if (searchLine == "") {
        std::cout << "El archivo no se encuentra resgistrado" << std::endl;
        return;
    }
    // comprocacion de condicion
    int sizeContidions = 0;
    std::string stringConditions, stringTypeConditions, stringOperator, stringNumber;
    bool searchWord = true;
    int index = 0;
    // falta ajustar la funcion para que si recive un string puede aggarar mas de una palbra talvez usando comillas
    if (condition != "") {
        sizeContidions = sizeString(condition, ' ');
        if (sizeContidions != 3) {
            std::cout << "Error en las condiciones" << std::endl;
            return;
        }
        std::istringstream ssc(condition), sfs(this->searchLine);
        std::string wordLine;
        std::getline(sfs, wordLine, '#');
        std::getline(ssc, stringConditions, ' ');
        while (std::getline(sfs, wordLine, '#')) {
            index++;
            if (wordLine == stringConditions && std::getline(sfs, stringTypeConditions, '#')) {
                // mejorar
                if (stringTypeConditions == "int" || stringTypeConditions == "float" || stringTypeConditions == "bool") {
                    searchWord = false;
                    this->isString = false;
                    break;
                }
                if (stringTypeConditions == "string" || stringTypeConditions == "char") {
                    this->isString = true;
                    searchWord = false;
                    break;
                }
            }
            std::getline(sfs, wordLine, '#');
            std::getline(sfs, wordLine, '#');
        }
        if (searchWord && this->isString == false) {
            std::cout << stringTypeConditions << std::endl;
            std::cout << "El dato de la condiciones no se encuentra" << std::endl;
            return;
        }
        std::getline(ssc, stringOperator, ' ');
        if (!(stringOperator == "<" || stringOperator == ">" || stringOperator == "==")) {
            std::cout << "El operador registro no se encuentra disponible" << std::endl;
            return;
        }
        std::getline(ssc, stringNumber, ' ');
        if (!convertToNumber(stringNumber, "float") && this->isString == false) {
            std::cout << "Solo maneja numeros" << std::endl;
            return;
        }
    }
    int sizeArchive = sizeString(searchLine, '#');
    std::string namearchive = "out/" + archive + ".txt";
    std::fstream archiveTable(namearchive, std::ios::in);
    if (!archiveTable.is_open()) {
        std::cout << "Error en abrir el archivo de la tabla" << std::endl;
        return;
    }
    bool pass = false;
    std::fstream archiveToPass;
    if (toPass != "") {
        std::string schemeToPass = searchSheme(toPass);
        if (columns[0] == '*') {
            size_t posHastag = searchLine.find("#");
            std::string partLine = toPass + this->searchLine.substr(posHastag);
            std::fstream scheme(this->database, std::ios::out | std::ios::app);
            if (!scheme.is_open()) {
                std::cout << "Error al abrir el archivo de esquemas" << std::endl;
                return;
            }
            if (schemeToPass == "") {
                scheme << partLine << std::endl;
            } else {
                std::cout << "La tabla a crear ya se cuenta registrada tomar eso en cuenta." << std::endl;
            }
            scheme.close();
        } else {
            // modificar luego para la nueva forma de almacena los datos
            if (searchSheme(toPass) != "") {
                std::cout << "Esta tabla ya existe" << std::endl;
                return;
            }
            // recorre la column a traves de un getline y buscar que este en el esquema y guardar la siguetne palabra en un string
            std::string oldString, newString, strangeString, stringOfArchive, stringNumber;
            std::istringstream newStream(columns), newStreamArchive;
            while (std::getline(newStream, newString, ',')) {
                newStreamArchive.str(searchLine);
                std::getline(newStreamArchive, oldString, '#');
                while (std::getline(newStreamArchive, oldString, '#')) {
                    if (oldString == newString) {
                        std::getline(newStreamArchive, strangeString, '#');
                        std::getline(newStreamArchive, stringNumber, '#');
                        stringOfArchive += "#" + oldString + "#" + strangeString + "#" + stringNumber;
                        break;
                    }
                    std::getline(newStreamArchive, oldString, '#');
                    std::getline(newStreamArchive, stringNumber, '#');
                }
            }
            std::string partLine = toPass + stringOfArchive;
            std::fstream scheme(this->database, std::ios::out | std::ios::app);
            if (!scheme.is_open()) {
                std::cout << "Error al abrir el archivo de esquemas" << std::endl;
                return;
            }
            scheme << partLine << std::endl;
            scheme.close();
        }
        // arriba agregar comprobaciones para cosas de espeficos
        std::string nameToPass = "out/" + toPass + ".txt";
        archiveToPass.open(nameToPass, std::ios::out | std::ios::app);
        if (!archiveToPass.is_open()) {
            std::cout << "Error en abrir archivo a pasar" << std::endl;
            return;
        }
        pass = true;
    }
    // Agregar comprobacion correcta de conditions al toPass y para la condicional
    std::string lineTable;
    if (columns[0] == '*' && columns.size() == 1) {
        std::istringstream ssi(searchLine);
        std::string fisrt;
        std::getline(ssi, fisrt, '#');
        std::stringstream formString;
        if (!pass) {
            while (std::getline(ssi, fisrt, '#')) {
                formString << std::setw(COLUMN_WIDTH) << std::left << fisrt.substr(0, COLUMN_WIDTH - 2);
                for (int j = 0; j < 2; ++j) std::getline(ssi, fisrt, '#');
            }
            std::string stripes(COLUMN_WIDTH * sizeArchive, '-');
            std::cout << formString.str() << std::endl;
            std::cout << stripes << std::endl;
        }
        // Cosas de condicionales
        while (std::getline(archiveTable, lineTable)) {
            std::istringstream ss(lineTable);
            std::string momentWord, problemWord, stringFuture;
            std::stringstream formattedString;
            std::istringstream svss(this->searchLine);
            std::string stringPower = lineTable;
            if (!searchWord) {
                std::getline(svss, problemWord, '#');
                for (int i = 0; i < index; i++) {
                    for (int j = 0; j < 3; ++j) std::getline(svss, problemWord, '#');
                    stringFuture = stringPower.substr(0, std::stoi(problemWord));
                    stringPower = stringPower.substr(std::stoi(problemWord));
                }
            }
            std::string lineToPass;
            std::string forFuture = lineTable;
            std::istringstream sssv(this->searchLine);
            std::getline(sssv, momentWord, '#');
            while (std::getline(sssv, momentWord, '#')) {
                for (int j = 0; j < 2; ++j) std::getline(sssv, momentWord, '#');
                if (searchWord) {
                    if (pass) {
                        lineToPass += forFuture;
                        break;
                    } else {
                        std::string tempData = forFuture.substr(0, std::stoi(momentWord));
                        formattedString << std::setw(COLUMN_WIDTH) << std::left << tempData.substr(0, COLUMN_WIDTH - 2);
                        forFuture = forFuture.substr(std::stoi(momentWord));
                    }
                } else {  // sis searchWord es falso es decir hay condicional
                    if (isOnlySpaces(stringFuture))
                        break;
                    else if (checkParementer(stringFuture, stringOperator, stringNumber)) {
                        if (pass) {
                            lineToPass += forFuture;
                            break;
                        } else {
                            std::string tempData = forFuture.substr(0, std::stoi(momentWord));
                            formattedString << std::setw(COLUMN_WIDTH) << std::left << tempData.substr(0, COLUMN_WIDTH - 2);
                            forFuture = forFuture.substr(std::stoi(momentWord));
                        }
                    }
                }
            }
            if (lineToPass != "")
                archiveToPass << lineToPass << std::endl;
            else if (formattedString.str() != "")
                std::cout << formattedString.str() << std::endl;
        }
        archiveToPass.close();
        return;
    } else {
        int sizeColumn = sizeString(columns, ',');
        if (sizeColumn > sizeArchive) {
            std::cout << "Los paremetros de colummas sobrepasan a los de la tabla" << std::endl;
            return;
        }
        if (!haveTheWordsInScheme(columns, searchLine)) {
            std::cout << "No ingresa columnas que no existen en la tabla" << std::endl;
            return;
        }
        std::istringstream sws(columns);
        std::string wordColumn, lineNumber;
        std::stringstream formattedColumn;
        // agregar condiciona para sie sta el archivo
        while (std::getline(sws, wordColumn, ',')) {
            formattedColumn << std::setw(COLUMN_WIDTH) << std::left << wordColumn.substr(0, COLUMN_WIDTH - 2);
            lineNumber += getWordPositionOfLineScheme(wordColumn, searchLine, '#') + " ";
        }
        if (!pass) {
            std::string stripes(COLUMN_WIDTH * sizeColumn, '-');
            std::cout << formattedColumn.str() << std::endl;
            std::cout << stripes << std::endl;
        }
        while (std::getline(archiveTable, lineTable)) {
            std::istringstream ss(this->searchLine), ssn(lineNumber);
            std::string wordData, number, stringFuture, problemWord;
            std::stringstream formattedData;
            std::string stringPower = lineTable;
            if (!searchWord) {
                std::getline(ss, problemWord, '#');
                for (int i = 0; i < index; i++) {
                    for (int j = 0; j < 3; ++j) std::getline(ss, problemWord, '#');
                    stringFuture = stringPower.substr(0, std::stoi(problemWord));
                    stringPower = stringPower.substr(std::stoi(problemWord));
                }
            }
            // vista y pasar a archivo
            std::string lineToPass, momentWord;
            while (std::getline(ssn, number, ' ')) {
                std::istringstream sssv(this->searchLine);
                std::getline(sssv, momentWord, '#');
                std::string forFuture = lineTable;
                std::string tempData;
                if (searchWord) {
                    int indexNumber = std::stoi(number);
                    for (int i = 0; i < indexNumber; i++) {
                        for (int j = 0; j < 3; ++j) std::getline(sssv, momentWord, '#');
                        tempData = forFuture.substr(0, std::stoi(momentWord));
                        forFuture = forFuture.substr(std::stoi(momentWord));
                    }
                    if (pass)
                        lineToPass += tempData;
                    else
                        formattedData << std::setw(COLUMN_WIDTH) << std::left << tempData.substr(0, COLUMN_WIDTH - 2);
                } else {  // si searchword es falso es que hay condiciones
                    if (isOnlySpaces(stringFuture))
                        break;
                    else if (checkParementer(stringFuture, stringOperator, stringNumber)) {
                        int indexNumber = std::stoi(number);
                        for (int i = 0; i < indexNumber; i++) {
                            for (int j = 0; j < 3; ++j) std::getline(sssv, momentWord, '#');
                            tempData = forFuture.substr(0, std::stoi(momentWord));
                            forFuture = forFuture.substr(std::stoi(momentWord));
                        }
                        if (pass)
                            lineToPass += tempData;
                        else
                            formattedData << std::setw(COLUMN_WIDTH) << std::left << tempData.substr(0, COLUMN_WIDTH - 2);
                    }
                }
            }
            if (lineToPass != "")
                archiveToPass << lineToPass << std::endl;
            else if (formattedData.str() != "")
                std::cout << formattedData.str() << std::endl;
        }
        archiveToPass.close();
        return;
    }
}

bool SGBD::checkParementer(const std::string& number, const std::string& operatorSymbol, const std::string& numberToCheck) {
    if (!this->isString) {
        float numberOne = std::stof(number);
        float numberTwo = std::stof(numberToCheck);
        if (operatorSymbol == "<") {
            if (numberOne < numberTwo) {
                return true;
            }
        } else if (operatorSymbol == ">") {
            if (numberOne > numberTwo) {
                return true;
            }
        } else if (operatorSymbol == "==") {
            if (numberOne == numberTwo) {
                return true;
            }
        }
    } else {
        if (operatorSymbol == "==") {
            std::istringstream sso(number), ssf(numberToCheck);
            std::string wordOne, wordTwo;
            while (sso >> wordOne && ssf >> wordTwo) {
                if (wordOne != wordTwo) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

bool SGBD::haveTheWordsInScheme(const std::string& lineOne, const std::string& lineTwo) {
    std::istringstream sso(lineOne);
    std::string wordOne, wordTwo;
    while (std::getline(sso, wordOne, ',')) {
        bool found = false;
        std::istringstream sst(lineTwo);
        std::getline(sst, wordTwo, '#');
        while (std::getline(sst, wordTwo, '#')) {
            if (wordOne == wordTwo) {
                found = true;
                break;
            }
            std::getline(sst, wordTwo, '#');
            std::getline(sst, wordTwo, '#');
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

std::string SGBD::getWordPositionOfLineScheme(const std::string& word, const std::string& line, const char& symbol) {
    std::string words;
    std::istringstream ssi(line);
    std::getline(ssi, words, symbol);  // #
    int index = 0;
    while (std::getline(ssi, words, symbol)) {
        index++;
        if (words == word) {
            break;
        }
        std::getline(ssi, words, symbol);
        std::getline(ssi, words, symbol);
    }
    return std::to_string(index);
}

bool SGBD::haveSymbol(const std::string& word, const char& character) {
    return word.find(character) == std::string::npos;
}

bool SGBD::isOnlySpaces(const std::string& word) {
    return std::all_of(word.begin(), word.end(), [](char c) { return std::isspace(c); });
}

void SGBD::trimRight(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}