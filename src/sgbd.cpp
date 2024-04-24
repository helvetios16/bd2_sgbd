#include "../include/sgbd.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

void SGBD::addScheme(const std::string& archive, const std::string& variable) {
    std::fstream scheme("out/scheme.txt", std::ios::in | std::ios::out | std::ios::app);
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

void SGBD::addRegister(const std::string& archive, const std::string& variable) {
    std::fstream scheme("out/scheme.txt", std::ios::in);
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
    std::string nameArchive = "out/" + archive + ".txt";
    std::fstream archiveTable(nameArchive, std::ios::out | std::ios::app);
    archiveTable << variable << std::endl;
    archiveTable.close();
    // pasar los datos al documento
}

bool SGBD::validType(const std::string& type) {
    return (type == "string" || type == "int" || type == "float" || type == "char" || type == "bool");
}

int SGBD::sizeString(const std::string& line, const char& symbol) {
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

bool SGBD::checkType(const std::string& type, const std::string& variable) {
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

void SGBD::readCsv(const std::string& csv, const std::string& variable) {
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
    int sizeCsv = sizeString(firstLine, ',');
    int sizeVarible = sizeString(variable, ',');
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
    std::fstream scheme("out/scheme.txt", std::ios::out | std::ios::app);
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
    std::string namearchive = "out/" + archive + ".txt";
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

std::string SGBD::searchSheme(const std::string& archive) {
    std::fstream scheme("out/scheme.txt", std::ios::in);
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
    std::string searchLine = searchSheme(archive);
    if (searchLine == "") {
        std::cout << "El archivo no se encuentra resgistrado" << std::endl;
        return;
    }
    // comprocacion de condicion
    int sizeContidions = 0;
    std::string stringConditions;
    std::string stringTypeConditions;
    std::string stringOperator;
    std::string stringNumber;
    bool searchWord = true;
    int index = 0;
    if (condition != "") {
        sizeContidions = sizeString(condition, ' ');
        if (sizeContidions != 3) {
            std::cout << "Error en las condiciones" << std::endl;
            return;
        }
        std::istringstream ssc(condition);
        std::istringstream sfs(searchLine);
        std::string wordLine;
        std::getline(sfs, wordLine, '#');
        std::getline(ssc, stringConditions, ' ');
        while (std::getline(sfs, wordLine, '#')) {
            index++;
            if (wordLine == stringConditions && std::getline(sfs, stringTypeConditions, '#')) {
                if (stringTypeConditions == "int" || stringTypeConditions == "float" || stringTypeConditions == "bool") {
                    searchWord = false;
                    break;
                }
            }
            std::getline(sfs, wordLine, '#');
        }
        if (searchWord) {
            std::cout << "El dato de la condiciones no se encuentra" << std::endl;
            return;
        }
        std::getline(ssc, stringOperator, ' ');
        if (!(stringOperator == "<" || stringOperator == ">" || stringOperator == "==")) {
            std::cout << "El operador registro no se encuentra disponible" << std::endl;
            return;
        }
        std::getline(ssc, stringNumber, ' ');
        if (!convertToNumber(stringNumber, "float")) {
            std::cout << "Solo maneja numeros" << std::endl;
            return;
        }
    }
    const int columnWidth = 16;
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
        if (schemeToPass == "") {
            std::cout << "El archivo a pasar no se encuentra resgistrado" << std::endl;
            return;
        }
        // poner condicional para * y especificos
        if (columns[0] == '*') {
            if (sizeString(schemeToPass, '#') != sizeArchive) {
                std::cout << "Los paremtro entre ambos esquemas no coinciden" << std::endl;
                return;
            }
            std::string oldArchive, newArchive;
            std::istringstream sso(searchLine), ssn(schemeToPass);
            std::getline(sso, oldArchive, '#');
            std::getline(ssn, newArchive, '#');
            while (std::getline(sso, oldArchive, '#') && std::getline(ssn, newArchive, '#')) {
                std::getline(sso, oldArchive, '#');
                std::getline(ssn, newArchive, '#');
                if (oldArchive != newArchive) {
                    std::cout << "Los tipos de datos entre archivo no coinciden" << std::endl;
                    return;
                }
            }
        } else {
            if (sizeString(schemeToPass, '#') != sizeString(columns, ',')) {
                std::cout << "La cantidad de parametros coincide con los de la tabla a pasar" << std::endl;
                return;
            }
            // recorre la column a traves de un getline y buscar que este en el esquema y guardar la siguetne palabra en un string
            std::string oldString, newString, stringOfArchive;
            std::istringstream newStream(columns), newStreamArchive;
            while (std::getline(newStream, newString, ',')) {
                newStreamArchive.str(searchLine);
                std::getline(newStreamArchive, oldString, '#');
                while (std::getline(newStreamArchive, oldString, '#')) {
                    if (oldString == newString) {
                        std::getline(newStreamArchive, oldString, '#');
                        stringOfArchive += oldString + " ";
                        break;
                    }
                    std::getline(newStreamArchive, oldString, '#');
                }
            }
            // el el archivo a psar recorrer similar al anterior guardando el tipp de datos y lo guardo en otro string
            std::istringstream passToPass(schemeToPass);
            std::string stringToPass, oldStrinStr;
            std::getline(passToPass, oldStrinStr, '#');
            while (std::getline(passToPass, oldStrinStr, '#')) {
                std::getline(passToPass, oldStrinStr, '#');
                stringToPass += oldStrinStr + " ";
            }
            // el resultado sabre si comparo los strings
            if (stringToPass != stringOfArchive) {
                std::cout << "Los tipos de datos de los parametros no concuerdan la tabla a pasar" << std::endl;
                return;
            }
        }
        // arriba agregar comprobaciones para cosas de espeficos
        std::string nameToPass = "out/" + toPass + ".txt";
        archiveToPass.open(nameToPass, std::ios::out | std::ios::app);
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
                formString << std::setw(columnWidth) << std::left << fisrt.substr(0, columnWidth - 2);
                std::getline(ssi, fisrt, '#');
            }
            std::string stripes(columnWidth * sizeArchive, '-');
            std::cout << formString.str() << std::endl;
            std::cout << stripes << std::endl;
        }
        while (std::getline(archiveTable, lineTable)) {
            std::istringstream ss(lineTable);
            std::string momentWord;
            std::stringstream formattedString;
            std::istringstream ssb(lineTable);
            std::string stringFuture;
            if (!searchWord) {
                for (int i = 0; i < index; i++) {
                    std::getline(ssb, stringFuture, ',');
                    if (stringFuture.size() >= 1 && stringFuture.front() == '"' && stringFuture.back() != '"') {
                        size_t nextCommaPos = lineTable.find(',', ssb.tellg());
                        if (lineTable[nextCommaPos - 1] == '"') {
                            stringFuture += lineTable.substr(ssb.tellg(), nextCommaPos - ssb.tellg());
                            ssb.seekg(nextCommaPos + 1);
                        }
                    }
                }
            }
            int counterToComma = 0;
            std::string lineToPass;
            while (std::getline(ss, momentWord, ',')) {
                if (searchWord) {
                    if (momentWord.size() >= 1 && momentWord.front() == '"' && momentWord.back() != '"') {
                        size_t nextCommaPos = lineTable.find(',', ss.tellg());
                        if (lineTable[nextCommaPos - 1] == '"') {
                            momentWord += lineTable.substr(ss.tellg(), nextCommaPos - ss.tellg());
                            ss.seekg(nextCommaPos + 1);
                        }
                    }
                    if (pass) {
                        counterToComma++;
                        lineToPass += momentWord;
                        if (counterToComma != sizeArchive) {
                            lineToPass += ",";
                        }
                    } else {
                        formattedString << std::setw(columnWidth) << std::left << momentWord.substr(0, columnWidth - 2);
                    }
                } else {
                    if (stringFuture == "") {
                        break;
                    } else if (checkParementer(stringFuture, stringOperator, stringNumber)) {
                        if (momentWord.size() >= 1 && momentWord.front() == '"' && momentWord.back() != '"') {
                            size_t nextCommaPos = lineTable.find(',', ss.tellg());
                            if (lineTable[nextCommaPos - 1] == '"') {
                                momentWord += lineTable.substr(ss.tellg(), nextCommaPos - ss.tellg());
                                ss.seekg(nextCommaPos + 1);
                            }
                        }
                        if (pass) {
                            counterToComma++;
                            lineToPass += momentWord;
                            if (counterToComma != sizeArchive) {
                                lineToPass += ",";
                            }
                        } else {
                            formattedString << std::setw(columnWidth) << std::left << momentWord.substr(0, columnWidth - 2);
                        }
                    }
                }
            }
            if (lineToPass != "") {
                archiveToPass << lineToPass << std::endl;
            } else if (formattedString.str() != "") {
                std::cout << formattedString.str() << std::endl;
            }
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
        // hacer el resto primero sin where lurgo si luego para copiar a un archivo, falta copiar
        std::istringstream sws(columns);
        std::string wordColumn, lineNumber;
        std::stringstream formattedColumn;
        // agregar condiciona para sie sta el archivo
        while (std::getline(sws, wordColumn, ',')) {
            formattedColumn << std::setw(columnWidth) << std::left << wordColumn.substr(0, columnWidth - 2);
            lineNumber += getWordPositionOfLineScheme(wordColumn, searchLine, '#') + " ";
        }
        if (!pass) {
            std::string stripes(columnWidth * sizeColumn, '-');
            std::cout << formattedColumn.str() << std::endl;
            std::cout << stripes << std::endl;
        }
        while (std::getline(archiveTable, lineTable)) {
            std::istringstream ss(lineTable), ssn(lineNumber);
            std::string wordData, number, stringFuture;
            std::stringstream formattedData;
            // no olvidar agregar busqued apara conficioens usar ss
            if (!searchWord) {
                for (int i = 0; i < index; i++) {
                    std::getline(ss, stringFuture, ',');
                    if (stringFuture.size() >= 1 && stringFuture.front() == '"' && stringFuture.back() != '"') {
                        size_t nextCommaPos = lineTable.find(',', ss.tellg());
                        if (lineTable[nextCommaPos - 1] == '"') {
                            stringFuture += lineTable.substr(ss.tellg(), nextCommaPos - ss.tellg());
                            ss.seekg(nextCommaPos + 1);
                        }
                    }
                }
            }
            int counterToComma = 0;
            std::string lineToPass;
            while (std::getline(ssn, number, ' ')) {
                if (searchWord) {
                    int indexNumber = std::stoi(number);
                    std::istringstream sbs(lineTable);
                    for (int i = 0; i < indexNumber; i++) {
                        std::getline(sbs, wordData, ',');
                        if (wordData.size() >= 1 && wordData.front() == '"' && wordData.back() != '"') {
                            size_t nextCommaPos = lineTable.find(',', sbs.tellg());
                            if (lineTable[nextCommaPos - 1] == '"') {
                                wordData += lineTable.substr(sbs.tellg(), nextCommaPos - sbs.tellg());
                                sbs.seekg(nextCommaPos + 1);
                            }
                        }
                    }
                    if (pass) {
                        counterToComma++;
                        lineToPass += wordData;
                        if (counterToComma != sizeArchive) {
                            lineToPass += ",";
                        }
                    } else {
                        formattedData << std::setw(columnWidth) << std::left << wordData.substr(0, columnWidth - 2);
                    }
                } else {
                    if (stringFuture == "") {
                        break;
                    } else if (checkParementer(stringFuture, stringOperator, stringNumber)) {
                        int indexNumber = std::stoi(number);
                        std::istringstream sbs(lineTable);
                        for (int i = 0; i < indexNumber; i++) {
                            std::getline(sbs, wordData, ',');
                            if (wordData.size() >= 1 && wordData.front() == '"' && wordData.back() != '"') {
                                size_t nextCommaPos = lineTable.find(',', sbs.tellg());
                                if (lineTable[nextCommaPos - 1] == '"') {
                                    wordData += lineTable.substr(sbs.tellg(), nextCommaPos - sbs.tellg());
                                    sbs.seekg(nextCommaPos + 1);
                                }
                            }
                        }
                        if (pass) {
                            counterToComma++;
                            lineToPass += wordData;
                            if (counterToComma != sizeArchive) {
                                lineToPass += ",";
                            }
                        } else {
                            formattedData << std::setw(columnWidth) << std::left << wordData.substr(0, columnWidth - 2);
                        }
                    }
                }
            }
            if (lineToPass != "") {
                archiveToPass << lineToPass << std::endl;
            } else if (formattedData.str() != "") {
                std::cout << formattedData.str() << std::endl;
            }
        }
        archiveToPass.close();
        return;
    }
}

bool SGBD::checkParementer(const std::string& number, const std::string& operatorSymbol, const std::string& numberToCheck) {
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
    return false;
}
// linea con comas y linea con hashtag
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
    }
    return std::to_string(index);
}
