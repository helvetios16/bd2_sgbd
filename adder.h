#pragma once
#include <string>
class Adder {
   public:
    void addScheme(const std::string& archive, const std::string& variable);
    void addRegister(std::string archive);
    bool validType(const std::string& type);
    bool convertToNumber(const std::string& number);
};