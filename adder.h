#pragma once
#include <string>
class Adder {
   public:
    void addScheme(const std::string& archive, const std::string& variable);
    void addRegister(const std::string& archive, const std::string& variable);
    bool validType(const std::string& type);
    bool convertToNumber(const std::string& number);
    int sizeString(const std::string& line, const char& symbol);
    bool checkType(const std::string& type, const std::string& variable);
    bool convertToNumber(const std::string& number, const std::string& type);
};