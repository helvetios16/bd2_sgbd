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
    // sgbd.readCsv("netflix.csv", "string,string,string,float,date,int,int");
    // sgbd.see("Titanic", "Name,Age", "", "prueba");
    sgbd.see("prueba", "*", "", "");
    // sgbd.see("Titanic", "*", "Age == 54", "");
    return 0;
}
