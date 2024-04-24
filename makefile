all: compile run

compile:
	g++ -g main.cpp sgbd.cpp -o main.exe

run:
	main.exe

clean:
	rm *.exe