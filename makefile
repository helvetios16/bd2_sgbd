all: clean

main.exe: sgbd.o main.o
	g++ sgbd.o main.o -o main.exe

main.o: main.cpp include/sgbd.h
	g++ -c main.cpp

sgbd.o: src/sgbd.cpp include/sgbd.h
	g++ -c src/sgbd.cpp

run: main.exe
	./main.exe

clean: run
	rm *.o
