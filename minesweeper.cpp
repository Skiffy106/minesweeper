#include "minesweeper.h"

#include <curses.h>
#include <string.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include "helpers.h"

#define COLOR_NORMAL 1
#define COLOR_SELECT 2
#define SEED 69420
#define BOMB '*'
#define UNMAPPED ''

void Minesweeper::generateGame(int mode, int gameWidth, int gameHeight) {
  int numMines;
  int minesPlaced = 0;
  switch (mode) {
    case 0:
      /* Easy */
      numMines = 20;
      break;
    case 1:
      /* Medium */
      numMines = 30;
      break;
    case 2:
      /* Hard */
      numMines = 40;
      break;

    case 3:  // custom not implemented
    case 4:  // credits not implemented
    default:
      numMines = 40;
      break;
  }

  int **board;  // 2d array
  board = new int *[gameWidth];
  for (int i = 0; i < gameWidth; i++) {
    board[i] = new int[gameHeight];
  }

  while (minesPlaced < numMines) {
    int x = rand() % gameWidth;
    int y = rand() % gameHeight;
    if (board[x][y] != '*') {
      board[x][y] = '*';
      minesPlaced++;
    }
  }

  // second pass
  for (int i = 0; i < gameWidth; i++) {
    for (int j = 0; j < gameHeight; j++) {
      board[i][j] = getNeighbors(i, j, board);
    }
  }
  return board;
}

Minesweeper::Minesweeper() { srand(SEED); }

int Minesweeper::getNeighbors(int row, int col, int **data) {
  int count = 0;

  // above
  if (row - 1 >= 0) {
    if (col - 1 >= 0) {
      if (data[row - 1][col - 1] == BOMB) count++;
    }
    if (data[row - 1][col] == BOMB) count++;
    if (col + 1 >= 0) {
      if (data[row - 1][col + 1] == BOMB) count++;
    }
  }

  // left
  if (col - 1 >= 0) {
    if (data[row][col - 1] == BOMB) count++;
  }
  // right
  if (col + 1 >= 0) {
    if (data[row][col + 1] == BOMB) count++;
  }

  // below
  if (row + 1 >= 0) {
    if (col - 1 >= 0) {
      if (data[row + 1][col - 1] == BOMB) count++;
    }
    if (data[row + 1][col] == BOMB) count++;
    if (col + 1 >= 0) {
      if (data[row + 1][col + 1] == BOMB) count++;
    }
  }

  return count;
}

int Minesweeper::handleMenu() {
  int input, HEIGHT, WIDTH, x_offset;
  getmaxyx(stdscr, HEIGHT, WIDTH);
  const char options[][20] = {"Easy",   "Medium",  "Hard",
                              "Custom", "Credits", "Quit"};
  char msg[] = "You have chosen option #";
  const int numOptions = sizeof(options) / sizeof(options[0]);
  int selection = 0;

  int y_offset = (HEIGHT - numOptions) / 2;  // center menu in y

  init_pair(COLOR_NORMAL, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_SELECT, COLOR_BLACK, COLOR_WHITE);
  attron(COLOR_PAIR(COLOR_NORMAL));  // turn on white text

  clear();
  while (input != 10) {       // enter
    selection %= numOptions;  // wrap selection
    for (int i = 0; i < numOptions; i++) {
      x_offset = (WIDTH - strlen(options[i])) / 2;  // center text in x
      if (i == selection) attron(COLOR_PAIR(COLOR_SELECT));
      mvprintw(i + y_offset, x_offset, options[i]);
      attron(COLOR_PAIR(COLOR_NORMAL));
    }
    input = getch();
    switch (input) {
      case KEY_UP:
        selection--;
        if (selection == -1)
          selection = numOptions - 1;  // modulo is bad when negative
        break;
      case KEY_DOWN:
        selection++;
        break;
    }
  }
  clear();

  if (selection == 5) {
  }
  return selection;
}

void Minesweeper::playGame(int mode, int width, int height) {
  int **board = generateGame(mode, width, height);
  int input, cursorX, cursorY;
  while (input != KEY_ENTER) {  // enter
    // show board
    input = getch();
    switch (input) {
      case KEY_UP:
        break;
      case KEY_DOWN:
        break;
    }
  }
}
for (int j = 0; j < board[i].size(); j++) {
  move(left + i, top + j);
  addch(board[i][j]);
}
}
refresh();
}

void Minesweeper::playGame() {
  handleMenu();
  generateGame();
  int input, cursorX, cursorY;
  while (input != KEY_ENTER) {  // enter
    showBoard();
    input = getch();
    switch (input) {
      case KEY_UP:
        cursorY++;
        break;
      case KEY_DOWN:
        cursorY--;
        break;
      case KEY_LEFT:
        cursorX--;
        break;
      case KEY_RIGHT:
        cursorX++;
        break;
    }
  }
}
