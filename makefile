all: clean

main.exe: sgbd.o main.o
	g++ -g sgbd.o main.o -o main.exe

main.o: main.cpp include/sgbd.h
	g++ -c main.cpp

sgbd.o: src/sgbd.cpp include/sgbd.h
	g++ -c src/sgbd.cpp

run: main.exe
	./main.exe

clean: run
	rm *.o

debug:
	g++ -g src/sgbd.cpp main.cpp -o main.exe