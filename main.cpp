#include <iostream>

#include "include/disk.h"
#include "include/memory.h"
#include "include/sgbd.h"
#include "include/shell.h"

int main(int argc, char const *argv[]) {
    Disk disk;
    disk.remove();
    disk.setDisk();
    // disk.createDefault();
    Memory mery;
    mery.getBlocksToStack("disk");  // llamar solo una vez luego haver creado el disco
    mery.printVectorInArchive();    // llamar solo una vez luego haver creado el disco

    // disk.getMemory();
    // disk.createDefault();
    // disk.checkInformation();
    // disk.about();
    // Shell shell;
    // shell.shell();

    // mery.addInBlockRelationColumns("db-schema", "movie", "#edad#int#8");
    // mery.addInBlockRelation("db-schema", "movie");
    // mery.addInBlockDatabase("db-hola");
    // mery.addRegisterInSectors("db-schema", "titanic", "out/register.txt", 50);
    // mery.addInBlockTable();
    // std::cout << mery.getDatabaseOfBlock("db-schema") << std::endl;
    SGBD sgbd;
    sgbd.createDatabase("schema");
    sgbd.useDatabase("schema");
    sgbd.createTable("prueba_titanic");
    sgbd.addColumn("PassangerId int 8,Suvived int 8,Pclass int 8,Name string 64,Sex string 12,Age int 8,Sibsp int 8,Parch int 8,Ticket string 24,Fare float 16,Cabin string 16,Embarked char 1", "prueba_titanic");
    sgbd.showtable("prueba_titanic");
    sgbd.addCsvToTable("titanic.csv", "prueba_titanic");
    // sgbd.addRegister("prueba_titanic", "51,0,3,Arnold,female,18,1,0,349237,17.8,,S");
    // sgbd.see("prueba_titanic", "Name,Age", "", "");
    return 0;
}
