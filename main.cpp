#include <curses.h>

#include "helpers.h"
#include "minesweeper.h"

int main() {
  if (initializeScreen() != 0) closeScreen(1);

  Minesweeper game;
  game.playGame();

  closeScreen(0);
}

// cd dev/cpp/minesweeper