#pragma once
#include <string>

#include "adder.h"

class Lectore {
   private:
    Adder andi;

   public:
    void readCsv(const std::string& csv, const std::string& variable);
    std::string searchSheme(const std::string& csv);
    void see(const std::string& archive, const std::string& columns, const std::string& condition, const std::string& toPass);
    bool checkParementer(const std::string& operatorSymbol, const std::string& number, const std::string& numberToCheck);
    bool haveTheWordsInScheme(const std::string& lineOne, const std::string& lineTwo);
    std::string getWordPositionOfLineScheme(const std::string& word, const std::string& line, const char& symbol);
};