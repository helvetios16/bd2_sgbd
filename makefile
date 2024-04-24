all: compile run

compile:
	g++ -g main.cpp src/sgbd.cpp -o main.exe

run:
	main.exe

clean:
	rm *.exe