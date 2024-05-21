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

void SGBD::createDatabase(const std::string& db) {
    std::string database = memory.getDatabaseOfBlock("db-" + db);
    if (database == "")
        memory.addInBlockDatabase("db-" + db);
    else
        std::cout << "La base de datos ya ha sido registrada" << std::endl;
}

void SGBD::useDatabase(const std::string& db) {
    if (memory.getDatabaseOfBlock("db-" + db) == "") {
        std::cout << "La base de datos no ha sido registrada" << std::endl;
        return;
    }
    this->database = "db-" + db;
}

void SGBD::createTable(const std::string& archive) {
    if (this->database == "") {
        std::cout << "No se ha seleccionado una base de datos" << std::endl;
        return;
    }
    if (memory.getRelationOfBlock(this->database + "@" + archive) == "") {
        memory.addInBlockRelation(this->database, archive);
    } else {
        std::cout << "La tabla o relacion ya ha sido registrada" << std::endl;
    }
}

void SGBD::addColumn(const std::string& information, const std::string& archive) {
    if (this->database == "") {
        std::cout << "No se ha seleccionado una base de datos" << std::endl;
        return;
    }
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
                passLine += "#" + word + "#" + otherWord;
                std::getline(ssi, otherWord, ' ');
                if (!isdigit(otherWord[0])) {
                    std::cout << "Se tiene que registrar un numero para los numeros" << std::endl;
                    return;
                } else {
                    passLine += "#" + otherWord;
                }
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
    if (memory.getRelationOfBlock(this->database + "@" + archive) == "") {
        std::cout << "La tabla no se encuentra registrada" << std::endl;
        return;
    } else
        memory.addInBlockRelationColumns(this->database, archive, passLine);
}

void SGBD::showtable(const std::string& archive) {
    std::string searchLine = getLineToTable(archive);
    if (searchLine == "") return;
    std::stringstream formStringData;
    std::istringstream sss(searchLine);
    std::string word;
    std::getline(sss, word, '#');
    std::cout << word << std::endl;
    std::string stripes(COLUMN_WIDTH * 3, '-');
    std::cout << stripes << std::endl;
    while (std::getline(sss, word, '#')) {
        formStringData << std::setw(COLUMN_WIDTH) << std::left << word.substr(0, COLUMN_WIDTH - 2);
        std::getline(sss, word, '#');
        formStringData << std::setw(COLUMN_WIDTH) << std::left << word.substr(0, COLUMN_WIDTH - 2);
        std::getline(sss, word, '#');
        formStringData << std::setw(COLUMN_WIDTH) << std::left << word.substr(0, COLUMN_WIDTH - 2) << std::endl;
    }
    std::cout << formStringData.str();
}

void SGBD::addCsvToTable(const std::string& csv, const std::string& archive) {  // ajustar para que se puede agregar "n" registros
    std::string searchLine = getLineToTable(archive);
    if (searchLine == "") return;
    std::fstream archiveCsv("csv/" + csv, std::ios::in);
    if (!archiveCsv.is_open()) {
        std::cout << "Error al abrir el archivo csv" << std::endl;
        return;
    }
    std::string line, otherWord, word;
    std::getline(archiveCsv, line);
    int sizeFile = sizeString(searchLine, '#');
    if (sizeFile != sizeString(line, ',')) {
        std::cout << "La cantidad de columnas no coincide con la tabla" << std::endl;
        archiveCsv.close();
        return;
    }
    std::stringstream formString;
    int index = 0;
    while (std::getline(archiveCsv, line)) {
        std::istringstream ss(searchLine);
        std::istringstream sss(line);
        std::getline(ss, otherWord, '#');
        formString << this->database + "@" + archive + "Ø";
        while (std::getline(ss, otherWord, '#')) {
            for (int i = 0; i < 2; ++i) std::getline(ss, otherWord, '#');
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
        index++;
    }
    std::fstream archiveToPass("out/register.txt", std::ios::out);
    if (!archiveToPass.is_open()) {
        std::cout << "Error al abrir el archivo de la tabla" << std::endl;
        return;
    }
    archiveToPass << formString.str();
    archiveToPass.close();
    memory.addRegisterInSectors(this->database, archive, "out/register.txt", index);
}

void SGBD::addRegister(const std::string& archive, const std::string& variable) {
    std::string searchLine = getLineToTable(archive);
    int sizeVariable = sizeString(variable, ',');
    int sizeRelation = sizeString(searchLine, '#');
    if (sizeVariable != sizeRelation) {
        std::cout << "La cantidad de columnas no coincide con la tabla" << std::endl;
        return;
    }
    std::istringstream ssv(variable);
    std::istringstream sss(searchLine);
    std::string firstWord, secondWord, thirdWord, primaryWord, secondWordNext;
    std::getline(sss, primaryWord, '#');
    while (std::getline(sss, firstWord, '#')) {
        std::getline(sss, secondWord, '#');
        std::getline(sss, thirdWord, '#');
        std::getline(ssv, secondWordNext, ',');
        if (!checkType(secondWord, secondWordNext)) {
            std::cout << "Ingrese el dato correcto al tipo de variable" << std::endl;
            return;
        }
        if (secondWord == "string") {
            int sizeWord = secondWordNext.size();
            if (sizeWord > std::stoi(thirdWord)) {
                std::cout << "El tamaño de la cadena es mayor al permitido" << std::endl;
                return;
            }
            continue;
        }
    }
    std::string input, otherInput;
    std::stringstream formString;
    std::istringstream vvv(variable), svv(searchLine);
    std::getline(svv, input, '#');
    while (std::getline(svv, input, '#')) {
        for (int i = 0; i < 2; ++i) std::getline(svv, input, '#');
        std::getline(vvv, otherInput, ',');
        formString << std::setw(std::stoi(input)) << std::left << otherInput;
    }
    std::fstream archiveRegister("out/register.txt", std::ios::out);
    if (!archiveRegister.is_open()) {
        std::cout << "Error al abrir el archivo de la tabla" << std::endl;
        return;
    }
    archiveRegister << this->database << "@" << archive << "Ø" << formString.str() << std::endl;
    archiveRegister.close();
    memory.addRegisterInSectors(this->database, archive, "out/register.txt", 1);
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
    std::string searchLine = getLineToTable(archive);
    int sizeConditions = 0, index = 0;
    std::string stringConditions, stringTypeConditions, stringOperator, stringNumber;
    bool searchWord = true;
    if (condition != "") {
        sizeConditions = sizeString(condition, ' ');
        if (sizeConditions != 3) {
            std::cout << "Error en las condiciones" << std::endl;
            return;
        }
        std::istringstream ssc(condition), sfs(searchLine);
        std::string wordLine;
        std::getline(sfs, wordLine, '#');
        std::getline(ssc, stringConditions, ' ');
        while (std::getline(sfs, wordLine, '#')) {
            index++;
            if (wordLine == stringConditions && std::getline(sfs, stringTypeConditions, '#')) {
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
            std::cout << "El dato de la condiciones no se encuentra" << std::endl;
            return;
        }
        std::getline(ssc, stringOperator, ' ');
        if (!(stringOperator == "<" || stringOperator == ">" || stringOperator == "==" || stringOperator == "!=" || stringOperator == "<=" || stringOperator == ">=")) {
            std::cout << "El operador registro no se encuentra disponible" << std::endl;
            return;
        }
        std::getline(ssc, stringNumber, ' ');
        if (!convertToNumber(stringNumber, "float") && this->isString == false) {
            std::cout << "Solo maneja numeros" << std::endl;
            return;
        }
    }
    bool pass = false;
    if (toPass != "") {
        std::string relationToPass = memory.getRelationOfBlock(this->database + "@" + toPass);
        if (columns[0] == '*' && columns.size() == 1) {
            if (relationToPass == "") {
                size_t posHastag = searchLine.find("#");
                std::string partLine = searchLine.substr(posHastag);
                memory.addInBlockRelation(this->database, toPass);
                memory.addInBlockRelationColumns(this->database, toPass, partLine);
            } else {
                std::cout << "La relacion a crear ya se cuenta registrada tomar eso en cuenta." << std::endl;
                return;
            }
        } else {
            if (relationToPass != "") {
                std::cout << "Esta relacion ya existe" << std::endl;
                return;
            }
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
            std::string partLine = stringOfArchive;
            memory.addInBlockRelation(this->database, toPass);
            memory.addInBlockRelationColumns(this->database, toPass, partLine);
        }
        pass = true;
    }
    int counter = 0;
    if (columns[0] == '*' && columns.size() == 1) {
        std::string paths = memory.getRelationOfBlock(this->database + "@" + archive), path;
        std::istringstream ssi(paths);
        if (!pass) {
            std::istringstream sss(searchLine);
            std::string fisrt;
            std::stringstream formString;
            std::getline(sss, fisrt, '#');
            while (std::getline(sss, fisrt, '#')) {
                formString << std::setw(COLUMN_WIDTH) << std::left << fisrt.substr(0, COLUMN_WIDTH - 2);
                for (int j = 0; j < 2; ++j) std::getline(sss, fisrt, '#');
            }
            std::string stripes(COLUMN_WIDTH * sizeString(searchLine, '#'), '-');
            std::cout << formString.str() << std::endl;
            std::cout << stripes << std::endl;
        }
        std::fstream archiveTemp("out/register.txt", std::ios::out);
        if (!archiveTemp.is_open()) {
            std::cout << "Error al abrir el archivo de la tabla" << std::endl;
            return;
        }
        while (std::getline(ssi, path, '\n')) {
            std::fstream fileSector(path, std::ios::in);
            if (!fileSector.is_open()) {
                std::cout << "Error al abrir el archivo de la tabla" << std::endl;
                return;
            }
            std::string line;
            while (std::getline(fileSector, line)) {
                // Comprovacion de la tabla a buscar
                std::string total = line.substr(0, line.find("Ø"));  // ver si se puede agregar +2 en el find
                std::string general = this->database + "@" + archive;
                if (total == general) {
                    int sizeLine = total.size();
                    std::string lineNew = line.substr(sizeLine + 2);
                    std::string momentWord, problemWord, stringFuture;
                    std::stringstream formattedString;
                    std::istringstream svss(searchLine);
                    std::string stringPower = lineNew;
                    if (!searchWord) {
                        std::getline(svss, problemWord, '#');
                        for (int i = 0; i < index; i++) {
                            for (int j = 0; j < 3; ++j) std::getline(svss, problemWord, '#');
                            stringFuture = stringPower.substr(0, std::stoi(problemWord));
                            stringPower = stringPower.substr(std::stoi(problemWord));
                        }
                    }
                    std::string lineToPass;
                    std::string forFuture = lineNew;
                    std::istringstream sssv(searchLine);
                    std::getline(sssv, momentWord, '#');
                    while (std::getline(sssv, momentWord, '#')) {
                        for (int j = 0; j < 2; ++j) std::getline(sssv, momentWord, '#');
                        if (searchWord) {
                            if (pass) {
                                lineToPass += this->database + "@" + toPass + "Ø" + forFuture;
                                break;
                            } else {
                                std::string tempData = forFuture.substr(0, std::stoi(momentWord));
                                formattedString << std::setw(COLUMN_WIDTH) << std::left << tempData.substr(0, COLUMN_WIDTH - 2);
                                forFuture = forFuture.substr(std::stoi(momentWord));
                            }
                        } else {  // si searchWord es falso, hay condicional
                            if (isOnlySpaces(stringFuture)) break;
                            if (checkParementer(stringFuture, stringOperator, stringNumber)) {
                                if (pass) {
                                    lineToPass += this->database + "@" + toPass + "Ø" + forFuture;
                                    break;
                                } else {
                                    std::string tempData = forFuture.substr(0, std::stoi(momentWord));
                                    formattedString << std::setw(COLUMN_WIDTH) << std::left << tempData.substr(0, COLUMN_WIDTH - 2);
                                    forFuture = forFuture.substr(std::stoi(momentWord));
                                }
                            }
                        }
                    }
                    if (lineToPass != "") {
                        archiveTemp << lineToPass << std::endl;
                        counter++;
                    } else if (formattedString.str() != "") {
                        std::cout << formattedString.str() << std::endl;
                    }
                }
            }

            // } else {
            //     int sizeColumn = sizeString(columns, ',');
            //     if (sizeColumn > sizeArchive) {
            //         std::cout << "Los paremetros de colummas sobrepasan a los de la tabla" << std::endl;
            //         return;
            //     }
            //     if (!haveTheWordsInScheme(columns, searchLine)) {
            //         std::cout << "No ingresa columnas que no existen en la tabla" << std::endl;
            //         return;
            //     }
            //     std::istringstream sws(columns);
            //     std::string wordColumn, lineNumber;
            //     std::stringstream formattedColumn;
            //     // agregar condiciona para sie sta el archivo
            //     while (std::getline(sws, wordColumn, ',')) {
            //         formattedColumn << std::setw(COLUMN_WIDTH) << std::left << wordColumn.substr(0, COLUMN_WIDTH - 2);
            //         lineNumber += getWordPositionOfLineScheme(wordColumn, searchLine, '#') + " ";
            //     }
            //     if (!pass) {
            //         std::string stripes(COLUMN_WIDTH * sizeColumn, '-');
            //         std::cout << formattedColumn.str() << std::endl;
            //         std::cout << stripes << std::endl;
            //     }
            //     while (std::getline(archiveTable, lineTable)) {
            //         std::istringstream ss(this->searchLine), ssn(lineNumber);
            //         std::string wordData, number, stringFuture, problemWord;
            //         std::stringstream formattedData;
            //         std::string stringPower = lineTable;
            //         if (!searchWord) {
            //             std::getline(ss, problemWord, '#');
            //             for (int i = 0; i < index; i++) {
            //                 for (int j = 0; j < 3; ++j) std::getline(ss, problemWord, '#');
            //                 stringFuture = stringPower.substr(0, std::stoi(problemWord));
            //                 stringPower = stringPower.substr(std::stoi(problemWord));
            //             }
            //         }
            //         // vista y pasar a archivo
            //         std::string lineToPass, momentWord;
            //         while (std::getline(ssn, number, ' ')) {
            //             std::istringstream sssv(this->searchLine);
            //             std::getline(sssv, momentWord, '#');
            //             std::string forFuture = lineTable;
            //             std::string tempData;
            //             if (searchWord) {
            //                 int indexNumber = std::stoi(number);
            //                 for (int i = 0; i < indexNumber; i++) {
            //                     for (int j = 0; j < 3; ++j) std::getline(sssv, momentWord, '#');
            //                     tempData = forFuture.substr(0, std::stoi(momentWord));
            //                     forFuture = forFuture.substr(std::stoi(momentWord));
            //                 }
            //                 if (pass)
            //                     lineToPass += tempData;
            //                 else
            //                     formattedData << std::setw(COLUMN_WIDTH) << std::left << tempData.substr(0, COLUMN_WIDTH - 2);
            //             } else {  // si searchword es falso es que hay condiciones
            //                 if (isOnlySpaces(stringFuture))
            //                     break;
            //                 else if (checkParementer(stringFuture, stringOperator, stringNumber)) {
            //                     int indexNumber = std::stoi(number);
            //                     for (int i = 0; i < indexNumber; i++) {
            //                         for (int j = 0; j < 3; ++j) std::getline(sssv, momentWord, '#');
            //                         tempData = forFuture.substr(0, std::stoi(momentWord));
            //                         forFuture = forFuture.substr(std::stoi(momentWord));
            //                     }
            //                     if (pass)
            //                         lineToPass += tempData;
            //                     else
            //                         formattedData << std::setw(COLUMN_WIDTH) << std::left << tempData.substr(0, COLUMN_WIDTH - 2);
            //                 }
            //             }
            //         }
            //         if (lineToPass != "")
            //             archiveToPass << lineToPass << std::endl;
            //         else if (formattedData.str() != "")
            //             std::cout << formattedData.str() << std::endl;
            //     }
            //     archiveToPass.close();
            //     return;
            // }
            fileSector.close();
        }
        archiveTemp.close();
        memory.addRegisterInSectors(this->database, archive, "out/register.txt", counter);
    } else {
        if (sizeString(columns, ',') > sizeString(searchLine, '#')) {
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
        while (std::getline(sws, wordColumn, ',')) {
            formattedColumn << std::setw(COLUMN_WIDTH) << std::left << wordColumn.substr(0, COLUMN_WIDTH - 2);
            lineNumber += getWordPositionOfLineScheme(wordColumn, searchLine, '#') + " ";
        }
        if (!pass) {
            std::string stripes(COLUMN_WIDTH * sizeString(columns, ','), '-');
            std::cout << formattedColumn.str() << std::endl;
            std::cout << stripes << std::endl;
        }
        std::string paths = memory.getRelationOfBlock(this->database + "@" + archive), path;
        std::istringstream ssi(paths);
        std::fstream archiveTable("out/register.txt", std::ios::out);
        if (!archiveTable.is_open()) {
            std::cout << "Error al abrir el archivo de la tabla" << std::endl;
            return;
        }
        while (std::getline(ssi, path, '\n')) {
            std::fstream fileSector(path, std::ios::in);
            if (!fileSector.is_open()) {
                std::cout << "Error al abrir el archivo de la tabla" << std::endl;
                return;
            }
            std::string line;
            while (std::getline(fileSector, line)) {
                std::string total = line.substr(0, line.find("Ø"));
                std::string general = this->database + "@" + archive;
                if (total == general) {
                    int sizeLine = total.size();
                    std::string lineNew = line.substr(sizeLine + 2);
                    std::string wordData, number, stringFuture, problemWord;
                    std::stringstream formattedData;
                    std::string stringPower = lineNew;
                    std::istringstream ssvss(searchLine), ssn(lineNumber);
                    if (!searchWord) {
                        std::getline(ssvss, problemWord, '#');
                        for (int i = 0; i < index; i++) {
                            for (int j = 0; j < 3; ++j) std::getline(ssvss, problemWord, '#');
                            stringFuture = stringPower.substr(0, std::stoi(problemWord));
                            stringPower = stringPower.substr(std::stoi(problemWord));
                        }
                    }
                    std::string lineToPass, momentWord;
                    while (std::getline(ssn, number, ' ')) {
                        std::istringstream sssv(searchLine);
                        std::getline(sssv, momentWord, '#');
                        std::string forFuture = lineNew;
                        std::string tempData;
                        if (searchWord) {
                            int indexNumber = std::stoi(number);
                            for (int i = 0; i < indexNumber; i++) {
                                for (int j = 0; j < 3; ++j) std::getline(sssv, momentWord, '#');
                                tempData = forFuture.substr(0, std::stoi(momentWord));
                                forFuture = forFuture.substr(std::stoi(momentWord));
                            }
                            if (pass)
                                lineToPass += tempData;  // buscar luego la manera de resolver esto, creo tuve la ide de que se agregre normal y luego al final cuando se vaya a pasar le agrego al inicio los de la base de datos y la tabla
                            else
                                formattedData << std::setw(COLUMN_WIDTH) << std::left << tempData.substr(0, COLUMN_WIDTH - 2);
                        } else {  // si searchword es falso es que hay condiciones
                            if (isOnlySpaces(stringFuture)) break;
                            if (checkParementer(stringFuture, stringOperator, stringNumber)) {
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
                        archiveTable << this->database << "@" << toPass << "Ø" << lineToPass << std::endl;
                    else if (formattedData.str() != "")
                        std::cout << formattedData.str() << std::endl;
                }
            }
        }
    }
}

bool SGBD::checkParementer(const std::string& number, const std::string& operatorSymbol, const std::string& numberToCheck) {
    if (!this->isString) {
        float numberOne = std::stof(number);
        float numberTwo = std::stof(numberToCheck);
        if (operatorSymbol == "<") {
            return numberOne < numberTwo;
        } else if (operatorSymbol == ">") {
            return numberOne > numberTwo;
        } else if (operatorSymbol == "==") {
            return numberOne == numberTwo;
        } else if (operatorSymbol == "!=") {
            return numberOne != numberTwo;
        } else if (operatorSymbol == "<=") {
            return numberOne <= numberTwo;
        } else if (operatorSymbol == ">=") {
            return numberOne >= numberTwo;
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
        } else if (operatorSymbol == "!=") {
            std::istringstream sso(number), ssf(numberToCheck);
            std::string wordOne, wordTwo;
            while (sso >> wordOne && ssf >> wordTwo) {
                if (wordOne == wordTwo) {
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

std::string SGBD::getLineToTable(const std::string& archive) {
    if (this->database == "") {
        std::cout << "No se ha seleccionado una base de datos" << std::endl;
        return "";
    }
    std::string paths = memory.getRelationOfBlock(this->database + "@" + archive);
    if (paths == "") {
        std::cout << "La tabla no se encuentra registrada" << std::endl;
        return "";
    }
    std::stringstream formStringData;
    std::istringstream ss(paths);
    std::string path, searchLine;
    int sizeLost = 0;
    bool process = false;
    while (std::getline(ss, path, '\n')) {
        std::fstream archiveRelation(path, std::ios::in);
        if (!archiveRelation.is_open()) {
            std::cout << "Error al abrir el archivo de la tabla" << std::endl;
            return "";
        }
        std::string line;
        while (std::getline(archiveRelation, line)) {
            std::string temp = line.substr(0, line.find("Ø"));
            std::istringstream sss(line);
            std::string temp2;
            std::getline(sss, temp2, '#');
            temp2 = temp2.substr(temp.size() + 2);
            if (temp == database && temp2 == archive) {
                searchLine = line;
                sizeLost = temp.size() + 2;
                process = true;
                break;
            }
        }
        if (process) break;
    }
    searchLine = searchLine.substr(sizeLost);
    return searchLine;
}