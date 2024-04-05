default: compile

compile: main.cpp helpers.cpp minesweeper.cpp
	g++ main.cpp helpers.cpp minesweeper.cpp -lcurses -o main

run: main.o
	./main