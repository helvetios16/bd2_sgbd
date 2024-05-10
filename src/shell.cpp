#include "../include/shell.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void Shell::shell() {
    while (true) {
        std::string command;
        std::cout << ">> ";
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        } else if (command == "clear") {
            system("clear");
        } else if (command == "help") {
            std::cout << "Commands:" << std::endl;
            std::cout << "  clear - clear the screen" << std::endl;
            std::cout << "  exit - exit the shell" << std::endl;
        } else if (command != "") {
            std::vector<std::string> tokens;
            std::string token, adder;
            std::istringstream stream(command);
            while (std::getline(stream, token, ' ')) {
                if (tokens.size() > 0 && token.front() == '(' && token.back() != ')') {
                    adder = token;
                    while (std::getline(stream, token, ' ')) {
                        adder += " " + token;
                        if (adder.front() == '(' && adder.back() == ')') break;
                    }
                    token = adder;
                } else if (tokens.size() > 0 && token.front() == '"' && token.back() != '"') {
                    adder = token;
                    while (std::getline(stream, token, ' ')) {
                        adder += " " + token;
                        if (adder.front() == '(' && adder.back() == ')') break;
                    }
                    token = adder;
                }
                tokens.push_back(token);
            }
            if (tokens[0] == "create" || tokens[0] == "use" || tokens[0] == "alter" || tokens[0] == "describe" || tokens[0] == "load" || tokens[0] == "select") sgbdControl(tokens);
        }
    }
}

void Shell::sgbdControl(std::vector<std::string> tokens) {
    if (tokens.size() == 3) {
        if (tokens[0] == "create") {
            if (tokens[1] == "database") {
                if (tokens[2].front() == '"' && tokens[2].back() == '"') tokens[2] = tokens[2].substr(1, tokens[2].size() - 2);
                sgbd.createDatabase(tokens[2]);
            } else if (tokens[1] == "table") {
                if (!haveSymbol(tokens[2], '#')) {
                    std::cout << "La tabla no puede tener el caracter #" << std::endl;
                    return;
                }
                if (tokens[2].front() == '"' && tokens[2].back() == '"') tokens[2] = tokens[2].substr(1, tokens[2].size() - 2);
                sgbd.createTable(tokens[2]);
            }
        }
    } else if (tokens.size() == 2) {
        if (tokens[0] == "use") {
            if (tokens[1].front() == '"' && tokens[1].back() == '"') tokens[1] = tokens[1].substr(1, tokens[1].size() - 2);
            sgbd.useDatabase(tokens[1]);
        } else if (tokens[0] == "describe") {
            if (tokens[1].front() == '"' && tokens[1].back() == '"') tokens[1] = tokens[1].substr(1, tokens[1].size() - 2);
            sgbd.showtable(tokens[1]);
        }
    } else if (tokens.size() == 6) {
        if (tokens[0] == "load" && tokens[1] == "data" && tokens[3] == "into" && tokens[4] == "table") {
            if (tokens[2].front() == '"' && tokens[2].back() == '"') tokens[2] = tokens[2].substr(1, tokens[2].size() - 2);
            if (tokens[5].front() == '"' && tokens[5].back() == '"') tokens[5] = tokens[5].substr(1, tokens[5].size() - 2);
            sgbd.addCsvToTable(tokens[2], tokens[5]);
        }
    } else if (tokens.size() == 5) {
        if (tokens[0] == "alter" && tokens[1] == "table" && tokens[3] == "add") {
            if (!haveSymbol(tokens[4], '#')) {
                std::cout << "La tabla no puede tener el caracter #" << std::endl;
                return;
            }
            if (tokens[2].front() == '"' && tokens[2].back() == '"') tokens[2] = tokens[2].substr(1, tokens[2].size() - 2);
            if (tokens[4].front() == '(' && tokens[4].back() == ')') tokens[4] = tokens[4].substr(1, tokens[4].size() - 2);
            sgbd.addColumn(tokens[4], tokens[2]);
        } else if (tokens[0] == "insert" && tokens[1] == "into" && tokens[3] == "values") {
            if (tokens[2].front() == '"' && tokens[2].back() == '"') tokens[2] = tokens[2].substr(1, tokens[2].size() - 2);
            if (tokens[4].front() == '"' && tokens[4].back() == '"') tokens[4] = tokens[4].substr(1, tokens[4].size() - 2);
            sgbd.addRegister(tokens[2], tokens[4]);
        }
    } else if (tokens.size() >= 4) {
        if (tokens[0] == "select" && tokens[2] == "from") {
            if (haveSymbol(tokens[3], '#')) {
                if (tokens[3].front() == '"' && tokens[3].back() == '"') tokens[3] = tokens[3].substr(1, tokens[3].size() - 2);
                if (tokens.size() == 4)
                    sgbd.see(tokens[3], tokens[1], "", "");
                else if (tokens.size() >= 6) {
                    if (tokens[4] == "where") {
                        if (tokens[5].front() == '(' && tokens[5].back() == ')') tokens[5] = tokens[5].substr(1, tokens[5].size() - 2);
                        if (tokens.size() == 6)
                            sgbd.see(tokens[3], tokens[1], tokens[5], "");
                        else if (tokens.size() == 8 && tokens[6] == "|") {
                            if (haveSymbol(tokens[7], '#')) {
                                if (tokens[7].front() == '"' && tokens[7].back() == '"') tokens[7] = tokens[7].substr(1, tokens[7].size() - 2);
                                sgbd.see(tokens[3], tokens[1], tokens[5], tokens[7]);
                            } else {
                                std::cout << "La tabla no puede tener el caracter #" << std::endl;
                            }
                        }
                    } else if (tokens[4] == "|") {
                        if (haveSymbol(tokens[5], '#')) {
                            if (tokens[5].front() == '"' && tokens[5].back() == '"') tokens[5] = tokens[5].substr(1, tokens[5].size() - 2);
                            sgbd.see(tokens[3], tokens[1], "", tokens[5]);
                        } else {
                            std::cout << "La tabla no puede tener el caracter #" << std::endl;
                        }
                    }
                }
            }
        }
    }
}

bool Shell::haveSymbol(const std::string& str, const char& symbol) {
    return str.find(symbol) == std::string::npos;
}