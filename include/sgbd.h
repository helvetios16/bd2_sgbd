#pragma once
#include <string>
class SGBD {
   public:
    void addScheme(const std::string& archive, const std::string& variable);
    void addRegister(const std::string& archive, const std::string& variable);
    bool validType(const std::string& type);
    int sizeString(const std::string& line, const char& symbol);
    bool checkType(const std::string& type, const std::string& variable);
    bool convertToNumber(const std::string& number, const std::string& type);
    void readCsv(const std::string& csv, const std::string& variable);
    std::string searchSheme(const std::string& csv);
    void see(const std::string& archive, const std::string& columns, const std::string& condition, const std::string& toPass);
    bool checkParementer(const std::string& operatorSymbol, const std::string& number, const std::string& numberToCheck);
    bool haveTheWordsInScheme(const std::string& lineOne, const std::string& lineTwo);
    std::string getWordPositionOfLineScheme(const std::string& word, const std::string& line, const char& symbol);
    void shell();
};