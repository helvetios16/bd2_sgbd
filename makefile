all: compile run

compile:
	g++ -g main.cpp adder.cpp lectore.cpp -o main.exe

run:
	main.exe