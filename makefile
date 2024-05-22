main.exe: build/disk.o build/memory.o build/sgbd.o build/shell.o build/main.o
	g++ build/shell.o build/disk.o build/sgbd.o build/memory.o build/main.o -o main.exe

build/main.o: main.cpp | build
	g++ -c main.cpp -o build/main.o

build/sgbd.o: src/sgbd.cpp include/sgbd.h | build
	g++ -c src/sgbd.cpp -o build/sgbd.o

build/disk.o: src/disk.cpp include/disk.h | build
	g++ -c src/disk.cpp -o build/disk.o

build/memory.o: src/memory.cpp include/memory.h | build
	g++ -c src/memory.cpp -o build/memory.o

build/shell.o: src/shell.cpp include/shell.h | build
	g++ -c src/shell.cpp -o build/shell.o

build:
	mkdir -p build

run: main.exe
	./main.exe

clean:
	rm -rf build main.exe

debug: | build
	g++ -g src/sgbd.cpp src/disk.cpp src/memory.cpp src/shell.cpp main.cpp -o main.exe
	gdb main.exe
