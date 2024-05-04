main.exe: disk.o sgbd.o shell.o main.o
	g++ -g shell.o disk.o sgbd.o main.o -o main.exe

main.o: main.cpp include/sgbd.h
	g++ -c main.cpp

sgbd.o: src/sgbd.cpp include/sgbd.h
	g++ -c src/sgbd.cpp

disk.o: src/disk.cpp include/disk.h
	g++ -c src/disk.cpp

shell.o: src/shell.cpp include/shell.h
	g++ -c src/shell.cpp

run: main.exe
	./main.exe
	rm *.o

clean:
	rm *.o

debug:
	g++ -g src/sgbd.cpp src/disk.cpp src/shell.cpp main.cpp -o main.exe