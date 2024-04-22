#include <iostream>

#include "adder.h"
#include "lectore.h"

int main(int argc, char const *argv[]) {
    Adder adison;
    Lectore lu;
    // Adison proof
    // adison.addScheme("prueba", "nombre,string,edad,int");
    // adison.addRegister("pruesa", "sebas,19");
    // adison.addScheme("index", "name,string,age,int,student,bool");
    // adison.addRegister("index", "sebas,19,1");

    // Lu proof
    // std::string prueba = lu.searchSheme("prueba");
    // std::cout << prueba << std::endl;
    lu.readCsv("Titanic.csv", "int,int,int,string,string,float,int,int,string,float,string,char");
    return 0;
}
