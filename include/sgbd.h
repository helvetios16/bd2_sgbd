#pragma once
#include <string>
class SGBD {
   private:
    std::string searchLine;
    std::string database;
    bool isString = false;

   public:
    SGBD();
    void createDatabase(const std::string& db);
    void useDatabase(const std::string& db);
    void createTable(const std::string& archive);
    void addColumn(const std::string& information, const std::string& archive);
    void showtable(const std::string& archive);
    void addCsvToTable(const std::string& csv, const std::string& archive);
    void addRegister(const std::string& archive, const std::string& variable);
    bool validType(const std::string& type);
    int sizeString(const std::string& line, const char& symbol);
    bool checkType(const std::string& type, const std::string& variable);
    bool convertToNumber(const std::string& number, const std::string& type);
    std::string searchSheme(const std::string& csv);
    void see(const std::string& archive, const std::string& columns, const std::string& condition, const std::string& toPass);
    bool checkParementer(const std::string& operatorSymbol, const std::string& number, const std::string& numberToCheck);
    bool haveTheWordsInScheme(const std::string& lineOne, const std::string& lineTwo);
    std::string getWordPositionOfLineScheme(const std::string& word, const std::string& line, const char& symbol);
    bool haveSymbol(const std::string& word, const char& character);
};