default: compile

compile: main.cpp helpers.cpp minesweeper.cpp
	g++ main.cpp helpers.cpp minesweeper.cpp -lcurses -o main.o

run: main.o
	./main.o