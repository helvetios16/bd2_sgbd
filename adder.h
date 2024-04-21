#pragma once
#include <string>
class Adder {
   public:
    void addScheme(const std::string& archive);
    void addRegister(std::string archive);
    bool convertToNumber(const std::string& number);
};