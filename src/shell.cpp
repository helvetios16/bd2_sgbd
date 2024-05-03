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
            std::cout << "  help - show this help" << std::endl;
            std::cout << "  create - create a new database" << std::endl;
            std::cout << "  use - use a database" << std::endl;
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
    }
}

bool Shell::haveSymbol(const std::string& str, const char& symbol) {
    return str.find(symbol) == std::string::npos;
}