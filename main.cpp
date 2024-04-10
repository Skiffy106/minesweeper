#include <curses.h>

#include "helpers.h"
#include "minesweeper.h"

int main() {
  if (initializeScreen() != 0) return closeScreen(1);

  Minesweeper game;
  game.playGame();

  // getch();
  return closeScreen(0);
}

// cd dev/cpp/minesweeper