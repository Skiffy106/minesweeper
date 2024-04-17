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
#define NOBOMB ' '
#define CURSOR '@'

void Minesweeper::generateGame() {
  int minesPlaced = 0;

  while (minesPlaced < mines) {
    int x = rand() % game_width;
    int y = rand() % game_height;
    if (board[y][x] != BOMB) {
      board[y][x] = BOMB;
      minesPlaced++;
    } else {
      board[y][x] = NOBOMB;
    }
  }

  // second pass
  for (int i = 0; i < game_width; i++) {
    for (int j = 0; j < game_height; j++) {
      if (board[j][i] != BOMB) {
        board[j][i] = getNeighbors(j, i) + '0';
      }
    }
  }
}

Minesweeper::Minesweeper() {
  srand(SEED);
  // srand(SEED);
  initClass();
}

void Minesweeper::initClass() {
  getmaxyx(stdscr, height, width);

  cursorX = width / 2;
  cursorY = height / 2;

  game_width = 30;
  game_height = height < 30 ? height : 30;  // min(height, 30)

  board.resize(game_height);
  for (int i = 0; i < game_height; ++i) {
    board[i].resize(game_width, NOBOMB);
  }

  for (int i = 0; i < game_width; i++) {
    for (int j = 0; j < game_height; j++) {
      board[j][i] = NOBOMB;
    }
  }

  shown.resize(game_height);
  for (int i = 0; i < game_height; ++i) {
    shown[i].resize(game_width, false);
  }

  for (int i = 0; i < game_width; i++) {
    for (int j = 0; j < game_height; j++) {
      shown[j][i] = false;
    }
  }
}

Minesweeper::~Minesweeper() {}

int Minesweeper::getNeighbors(int row, int col) {
  int count = 0;

  // above
  if (row - 1 >= 0) {
    if (col - 1 >= 0) {
      if (board[row - 1][col - 1] == BOMB) count++;
    }
    if (board[row - 1][col] == BOMB) count++;
    if (col + 1 < game_width) {
      if (board[row - 1][col + 1] == BOMB) count++;
    }
  }

  // left
  if (col - 1 >= 0) {
    if (board[row][col - 1] == BOMB) count++;
  }
  // right
  if (col + 1 < game_width) {
    if (board[row][col + 1] == BOMB) count++;
  }

  // below
  if (row + 1 < game_height) {
    if (col - 1 >= 0) {
      if (board[row + 1][col - 1] == BOMB) count++;
    }
    if (board[row + 1][col] == BOMB) count++;
    if (col + 1 < game_width) {
      if (board[row + 1][col + 1] == BOMB) count++;
    }
  }

  return count;
}

void Minesweeper::handleMenu() {
  int input, x_offset;
  const char options[][10] = {"Easy",   "Medium",  "Hard",
                              "Custom", "Credits", "Quit"};
  const int numOptions = sizeof(options) / sizeof(options[0]);
  int selection = 0;

  int y_offset = (height - numOptions) / 2;  // center menu in y

  init_pair(COLOR_NORMAL, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_SELECT, COLOR_BLACK, COLOR_WHITE);
  attron(COLOR_PAIR(COLOR_NORMAL));  // turn on white text

  clear();
  while (input != 10 || input == ' ') {  // enter or space
    selection %= numOptions;             // wrap selection
    for (int i = 0; i < numOptions; i++) {
      x_offset = (width - strlen(options[i])) / 2;  // center text in x
      if (i == selection) attron(COLOR_PAIR(COLOR_SELECT));
      mvprintw(i + y_offset, x_offset, options[i]);
      // printf("%d", height);  // debug
      attron(COLOR_PAIR(COLOR_NORMAL));
    }
    refresh();
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

  // C++ is so weird
  char top[9] = "Made by:";
  char bottom[14] = "Mason Tuttle!";
  char message[28] = "Custom not implemented yet.";

  switch (selection) {
    case 0:
      /* Easy */
      mines = 30;
      break;
    case 1:
      /* Medium */
      mines = 40;
      break;
    case 2:
      /* Hard */
      mines = 50;
      break;

    case 3:  // custom not implemented
      clear();
      mvaddstr(height / 2 - 1, (width - strlen(message)) / 2, message);
      refresh();
      getch();
      return handleMenu();
    case 4:
      clear();
      mvaddstr(height / 2 - 1, (width - strlen(top)) / 2, top);
      mvaddstr(height / 2, (width - strlen(bottom)) / 2, bottom);
      refresh();
      getch();
      return handleMenu();
    case 5:  // quit
      return (void)closeScreen(0);
    default:
      mines = 30;
      break;
  }
}

void Minesweeper::showBoard() {
  int left = (width - game_width) / 2;
  int top = (height - game_height) / 2;

  clear();
  char *tmp = new char[50];

  move(0, 0);
  sprintf(tmp, "Cursor: %d %d", cursorX, cursorY);
  // sprintf(tmp, "%lu %lu", board.size(), board[0].size());
  addstr(tmp);
  delete[] tmp;

  // refresh();
  attron(COLOR_PAIR(COLOR_SELECT));
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      move(top + i, left + j);
      // move(top, left);
      if (shown[i][j]) {
        addch(board.at(i).at(j));
      } else {
        addch(NOBOMB);
      }
    }
  }
  attron(COLOR_PAIR(COLOR_NORMAL));

  refresh();
}

void Minesweeper::showCursor() {
  // show cursor
  attron(COLOR_PAIR(COLOR_SELECT));
  move(cursorY, cursorX);
  addch(CURSOR);
  attron(COLOR_PAIR(COLOR_NORMAL));
}

void Minesweeper::showLose() {
  char top[10] = "You Lost!";
  char bottom[28] = "Press any key to try again.";
  clear();
  mvaddstr(height / 2 - 1, (width - strlen(top)) / 2, top);
  mvaddstr(height / 2, (width - strlen(bottom)) / 2, bottom);
  refresh();
  getch();
}

void Minesweeper::showWin() {
  attron(COLOR_PAIR(COLOR_SELECT));
  char top[10] = "You Won!";
  char bottom[28] = "Press any key to try again.";
  clear();
  mvaddstr(height / 2 - 1, (width - strlen(top)) / 2, top);
  mvaddstr(height / 2, (width - strlen(bottom)) / 2, bottom);
  refresh();
  attron(COLOR_PAIR(COLOR_NORMAL));
  getch();
}

void Minesweeper::playGame() {
  handleMenu();
  generateGame();
  showBoard();
  showCursor();
  int input;

  int left = (width - game_width) / 2;
  int top = (height - game_height) / 2;
  while (input != 27 && input != (int)'q') {  // esc or q
    showBoard();
    showCursor();
    input = getch();
    switch (input) {
      case KEY_UP:
        cursorY--;
        break;
      case KEY_DOWN:
        cursorY++;
        break;
      case KEY_LEFT:
        cursorX--;
        break;
      case KEY_RIGHT:
        cursorX++;
        break;
    }

    if (cursorX < left) cursorX = left + game_width - 1;
    if (cursorX >= left + game_width) cursorX = left;
    if (cursorY < top) cursorY = top + game_height - 1;
    if (cursorY >= top + game_height) cursorY = top;

    if (input == 10 || input == ' ') {  // enter
      // local -> global coords

      shown[cursorY - top][cursorX - left] = true;
      floodFill(cursorX - left, cursorY - top);

      if (board[cursorY - top][cursorX - left] == BOMB) {
        showLose();
        initClass();
        srand(time(nullptr));
        return playGame();
      }

      if (isWon()) {
        showWin();
        initClass();
        srand(time(nullptr));
        return playGame();
      }
    }
  }
}

bool Minesweeper::isWon() {
  int hiddenCounter = 0;
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      if (!shown[i][j]) {
        hiddenCounter++;
      }
    }
  }
  if (hiddenCounter == mines) {
    return true;
  }
  return false;
}

void Minesweeper::floodFill(int x, int y) {
  if (x < 0 || x >= game_width || y < 0 || y >= game_height) return;

  // show cell  (y, x)
  shown[y][x] = true;

  // Recursively call for north, east, south and west
  if (board[y][x] != '0') return;
  if (x + 1 < game_width && !shown[y][x + 1]) floodFill(x + 1, y);
  if (x - 1 >= 0 && !shown[y][x - 1]) floodFill(x - 1, y);
  if (y + 1 < game_height && !shown[y + 1][x]) floodFill(x, y + 1);
  if (y - 1 >= 0 && !shown[y - 1][x]) floodFill(x, y - 1);

  // corners
  if (x - 1 >= 0 && y - 1 >= 0 && !shown[y - 1][x - 1]) floodFill(x - 1, y - 1);
  if (x - 1 >= 0 && y + 1 < game_height && !shown[y + 1][x - 1])
    floodFill(x - 1, y + 1);
  if (x + 1 < game_width && y - 1 >= 0 && !shown[y - 1][x + 1])
    floodFill(x + 1, y - 1);
  if (x + 1 < game_width && y + 1 < game_height && !shown[y + 1][x + 1])
    floodFill(x + 1, y + 1);
}