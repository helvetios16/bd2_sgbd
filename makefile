main.exe: disk.o  memory.o sgbd.o shell.o main.o
	g++ shell.o disk.o sgbd.o memory.o main.o -o main.exe

main.o: main.cpp
	g++ -c main.cpp

sgbd.o: src/sgbd.cpp include/sgbd.h
	g++ -c src/sgbd.cpp

disk.o: src/disk.cpp include/disk.h
	g++ -c src/disk.cpp

memory.o: src/memory.cpp include/memory.h
	g++ -c src/memory.cpp

shell.o: src/shell.cpp include/shell.h
	g++ -c src/shell.cpp

run: main.exe
	./main.exe

clean:
	rm *.o

debug:
	g++ -g src/sgbd.cpp src/disk.cpp src/shell.cpp main.cpp -o main.exe
	gdb main.exe
