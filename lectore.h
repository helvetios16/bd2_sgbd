#pragma once
#include <string>

class Lectore {
   public:
    void readCsv(const std::string& csv, const std::string& variable);
    std::string searchSheme(const std::string& csv);
    void see(const std::string& archive, const std::string& columns, const std::string& condition, std::string& toPass);
};