#include <iostream>

#include "include/sgbd.h"

int main(int argc, char const *argv[]) {
    SGBD sgbd;
    // sgbd.see("Titanic", "Name,Age", "", "prueba");
    // sgbd.see("Titanic", "Name,Survived",  "Survived == 1", "student");
    // sgbd.see("student", "*", "", "");
    // sgbd.addScheme("intento1", "numero,int");
    // sgbd.addRegister("intento1", "10");
    // sgbd.see("intento1", "*", "", "");
    sgbd.readCsv("programming language trend over time.csv", "date,int,int,int");
    sgbd.see("programming language trend over time", "*", "Python > 95", "");
    // sgbd.see("Titanic", "Name,Age", "", "prueba");
    // sgbd.see("netflix", "title,imdb_score,runtime", "imdb_score < 5", "");
    // sgbd.see("Titanic", "*", "Age == 54", "");
    return 0;
}
