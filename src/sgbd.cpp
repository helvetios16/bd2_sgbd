#include "../include/sgbd.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

void SGBD::createTable(const std::string& archive) {
    std::string file = searchSheme(archive);
    if (file == "") {
        std::fstream scheme("out/scheme.txt", std::ios::out | std::ios::app);
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
    std::string line, word, passLine;
    while (std::getline(ss, line, ',')) {
        int index = 0;
        std::istringstream ssi(line);
        while (ssi >> word) {
            index++;
            if (index % 2 == 0) {
                if (!(word == "string" || word == "int" || word == "float" || word == "date" || word == "bool")) {
                    std::cout << "Tiene que ingresar un tipo de valor a la columna" << std::endl;
                    return;
                }
            }
            passLine += "#" + word;
        }
    }
    std::fstream scheme("out/scheme.txt", std::ios::in | std::ios::out);
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
    std::remove("out/scheme.txt");
    std::rename("out/temp.txt", "out/scheme.txt");
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
    const int columnWidth = 16;
    std::getline(ss, word, '#');
    std::cout << word << std::endl;
    std::string stripes(columnWidth * 2, '-');
    std::cout << stripes << std::endl;
    while (std::getline(ss, word, '#')) {
        formString << std::setw(columnWidth) << std::left << word.substr(0, columnWidth - 2);
        std::getline(ss, word, '#');
        formString << std::setw(columnWidth) << std::left << word.substr(0, columnWidth - 2) << std::endl;
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
    std::string line;
    std::getline(archiveCsv, line);
    int sizeFile = sizeString(file, '#');
    if (sizeFile != sizeString(line, ',')) {
        std::cout << "La cantidad de columnas no coincide con la tabla" << std::endl;
        archiveCsv.close();
        archiveTable.close();
        return;
    }
    while (std::getline(archiveCsv, line)) {
        archiveTable << line << std::endl;
    }
    archiveTable.close();
    archiveCsv.close();
}

void SGBD::addSchemeAllDirect(const std::string& archive, const std::string& variable) {
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

void SGBD::readCsvAllDirect(const std::string& csv, const std::string& variable) {
    // separar el punto csv
    size_t pos = csv.rfind('.');
    std::string archive;
    if (pos != std::string::npos) {
        archive = csv.substr(0, pos);
    }
    // comprovar que no este en el equema es csv
    this->searchLine = searchSheme(archive);
    if (searchLine != "") {
        std::cout << "El archivo csv ya se ha leido" << std::endl;
        return;
    }
    // pasar los datos de las columas al esquema
    std::fstream archiveCsv("csv/" + csv, std::ios::in);
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
        if (validType(word)) {
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
    this->searchLine = searchSheme(archive);
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
        if (columns[0] == '*') {
            size_t posHastag = searchLine.find("#");
            std::string partLine = toPass + this->searchLine.substr(posHastag);
            std::fstream scheme("out/scheme.txt", std::ios::out | std::ios::app);
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
                bool endOfLine = false;
                for (int i = 0; i < index; i++) {
                    std::getline(ssb, stringFuture, ',');
                    if (stringFuture.size() >= 1 && stringFuture.front() == '"' && stringFuture.back() != '"') {
                        size_t nextCommaPos = lineTable.find("\",", ssb.tellg());
                        if (nextCommaPos != std::string::npos) {
                            stringFuture += "," + lineTable.substr(ssb.tellg(), nextCommaPos - ssb.tellg() + 1);
                            ssb.seekg(nextCommaPos + 2);
                        } else {
                            stringFuture += "," + lineTable.substr(ssb.tellg());
                            endOfLine = true;
                        }
                    }
                    if (endOfLine) break;
                }
            }
            int counterToComma = 0;
            std::string lineToPass;
            while (std::getline(ss, momentWord, ',')) {
                bool endOfLine = false;
                if (searchWord) {
                    if (momentWord.size() >= 1 && momentWord.front() == '"' && momentWord.back() != '"') {
                        size_t nextCommaPoss = lineTable.find("\",", ss.tellg());
                        if (nextCommaPoss != std::string::npos) {
                            momentWord += "," + lineTable.substr(ss.tellg(), nextCommaPoss - ss.tellg() + 1);
                            ss.seekg(nextCommaPoss + 2);
                        } else {
                            momentWord += "," + lineTable.substr(ss.tellg());
                            endOfLine = true;
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
                    if (endOfLine) break;
                } else {
                    if (stringFuture == "") {
                        break;
                    } else if (checkParementer(stringFuture, stringOperator, stringNumber)) {
                        if (momentWord.size() >= 1 && momentWord.front() == '"' && momentWord.back() != '"') {
                            size_t nextCommaPoss = lineTable.find("\",", ss.tellg());
                            if (nextCommaPoss != std::string::npos) {
                                momentWord += "," + lineTable.substr(ss.tellg(), nextCommaPoss - ss.tellg() + 1);
                                ss.seekg(nextCommaPoss + 2);
                            } else {
                                momentWord += "," + lineTable.substr(ss.tellg());
                                endOfLine = true;
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
                        if (endOfLine) break;
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
                bool endOfLine = false;
                for (int i = 0; i < index; i++) {
                    std::getline(ss, stringFuture, ',');
                    if (stringFuture.size() >= 1 && stringFuture.front() == '"' && stringFuture.back() != '"') {
                        size_t nextCommaPos = lineTable.find("\",", ss.tellg());
                        if (nextCommaPos != std::string::npos) {
                            stringFuture += "," + lineTable.substr(ss.tellg(), nextCommaPos - ss.tellg() + 1);
                            ss.seekg(nextCommaPos + 2);
                        } else {
                            stringFuture += "," + lineTable.substr(ss.tellg());
                            endOfLine = true;
                        }
                    }
                    if (endOfLine) break;
                }
            }
            int counterToComma = 0;
            std::string lineToPass;
            while (std::getline(ssn, number, ' ')) {
                bool endOfLine = false;
                if (searchWord) {
                    int indexNumber = std::stoi(number);
                    std::istringstream sbs(lineTable);
                    for (int i = 0; i < indexNumber; i++) {
                        std::getline(sbs, wordData, ',');
                        if (wordData.size() >= 1 && wordData.front() == '"' && wordData.back() != '"') {
                            size_t nextCommaPos = lineTable.find("\",", sbs.tellg());
                            if (nextCommaPos != std::string::npos) {
                                wordData += "," + lineTable.substr(sbs.tellg(), nextCommaPos - sbs.tellg() + 1);
                                sbs.seekg(nextCommaPos + 2);
                            } else {
                                wordData += "," + lineTable.substr(sbs.tellg());
                                endOfLine = true;
                            }
                        }
                    }
                    if (pass) {
                        counterToComma++;
                        lineToPass += wordData;
                        if (counterToComma != sizeString(columns, ',')) {
                            lineToPass += ",";
                        }
                    } else {
                        formattedData << std::setw(columnWidth) << std::left << wordData.substr(0, columnWidth - 2);
                    }
                    if (endOfLine) break;
                } else {
                    if (stringFuture == "") {
                        break;
                    } else if (checkParementer(stringFuture, stringOperator, stringNumber)) {
                        int indexNumber = std::stoi(number);
                        std::istringstream sbs(lineTable);
                        for (int i = 0; i < indexNumber; i++) {
                            std::getline(sbs, wordData, ',');
                            if (wordData.size() >= 1 && wordData.front() == '"' && wordData.back() != '"') {
                                size_t nextCommaPos = lineTable.find("\",", sbs.tellg());
                                if (nextCommaPos != std::string::npos) {
                                    wordData += "," + lineTable.substr(sbs.tellg(), nextCommaPos - sbs.tellg() + 1);
                                    sbs.seekg(nextCommaPos + 2);
                                } else {
                                    wordData += "," + lineTable.substr(sbs.tellg());
                                    endOfLine = true;
                                }
                            }
                        }
                        if (pass) {
                            counterToComma++;
                            lineToPass += wordData;
                            if (counterToComma != sizeString(columns, ',')) {
                                lineToPass += ",";
                            }
                        } else {
                            formattedData << std::setw(columnWidth) << std::left << wordData.substr(0, columnWidth - 2);
                        }
                        if (endOfLine) break;
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
            return number == numberToCheck;
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

void SGBD::shell() {
    while (true) {
        std::string search;
        std::cout << ">> ";
        std::getline(std::cin, search);
        if (search == "exit") {
            break;
        } else if (search == "ls") {
            system("ls");
        } else if (search == "clear") {
            system("clear");
        } else if (search != "") {
            std::istringstream ss(search);
            std::string word, nextWord;
            std::vector<std::string> chain;
            while (ss >> word) {
                if (chain.size() > 0 && word.front() == '(' && word.back() != ')') {
                    nextWord = word;
                    while (ss >> word) {
                        nextWord += " " + word;
                        if (nextWord.front() == '(' && nextWord.back() == ')') break;
                    }
                    word = nextWord;
                }
                if (chain.size() > 0 && word.front() == '"' && word.back() != '"') {
                    nextWord = word;
                    while (ss >> word) {
                        nextWord += " " + word;
                        if (nextWord.front() == '"' && nextWord.back() == '"') break;
                    }
                    word = nextWord;
                }
                chain.push_back(word);
            }
            if (chain[0] == "read" && chain.size() == 3) {
                if (chain[1].front() == '"' && chain[1].back() == '"') {
                    chain[1] = chain[1].substr(1, chain[1].size() - 2);
                }
                size_t csvPos = chain[1].find(".csv");
                if (csvPos != std::string::npos && csvPos > 0 && csvPos == chain[1].size() - 4) {
                    if (chain[2].front() == '(' && chain[2].back() == ')') {
                        chain[2] = chain[2].substr(1, chain[2].size() - 2);
                        readCsvAllDirect(chain[1], chain[2]);
                    }
                }
            } else if (chain[0] == "create" && chain.size() == 4) {
                if (chain[1] == "scheme") {
                    if (haveSymbol(chain[2], '#')) {
                        if (chain[2].front() == '"' && chain[2].back() == '"') chain[2] = chain[2].substr(1, chain[2].size() - 2);
                        if (chain[3].front() == '(' && chain[3].back() == ')') {
                            chain[3] = chain[3].substr(1, chain[3].size() - 2);
                            addSchemeAllDirect(chain[2], chain[3]);
                        }
                    }
                } else {
                    std::cout << "No ingresar '#' en el nombre de la tabla" << std::endl;
                }
            } else if (chain[0] == "add" && chain.size() == 4) {
                if (chain[1] == "data") {
                    if (chain[2].front() == '"' && chain[2].back() == '"') chain[2] = chain[2].substr(1, chain[2].size() - 2);
                    if (chain[3].front() == '(' && chain[3].back() == ')') {
                        chain[3] = chain[3].substr(1, chain[3].size() - 2);
                        addRegister(chain[2], chain[3]);
                    }
                }
            } else if (chain[0] == "select" && chain.size() >= 4) {
                if (chain[1].front() == '(' && chain[1].back() == ')') {
                    chain[1] = chain[1].substr(1, chain[1].size() - 2);
                }
                if (chain[2] == "from") {
                    if (haveSymbol(chain[3], '#')) {
                        if (chain[3].front() == '"' && chain[3].back() == '"') chain[3] = chain[3].substr(1, chain[3].size() - 2);
                        if (chain.size() == 4) {
                            see(chain[3], chain[1], "", "");
                        } else if (chain.size() >= 6) {
                            if (chain[4] == "where") {
                                if (chain[5].front() == '(' && chain[5].back() == ')') {
                                    chain[5] = chain[5].substr(1, chain[5].size() - 2);
                                    if (chain.size() == 6) {
                                        see(chain[3], chain[1], chain[5], "");
                                    } else if (chain.size() == 8) {
                                        if (chain[6] == "|" || chain[6] == "->") {
                                            if (haveSymbol(chain[7], '#')) {
                                                see(chain[3], chain[1], chain[5], chain[7]);
                                            } else {
                                                std::cout << "No ingresar '#' en el nombre de la tabla" << std::endl;
                                            }
                                        }
                                    }
                                }
                            } else if (chain[4] == "|" || chain[4] == "->") {
                                std::cout << "yes" << std::endl;
                                if (haveSymbol(chain[5], '#')) {
                                    if (chain[5].front() == '"' && chain[5].back() == '"') chain[5] = chain[5].substr(1, chain[5].size() - 2);
                                    see(chain[3], chain[1], "", chain[5]);
                                } else {
                                    std::cout << "No ingresar '#' en el nombre de la tabla" << std::endl;
                                }
                            }
                        }
                    } else {
                        std::cout << "No ingresar '#' en el nombre de la tabla" << std::endl;
                    }
                }
            }
        }
    }
}

bool SGBD::haveSymbol(const std::string& word, const char& character) {
    return word.find(character) == std::string::npos;
}