all: compile baserun

compile:
	g++ -g main.cpp adder.cpp -o main.exe

run:
	main.exe