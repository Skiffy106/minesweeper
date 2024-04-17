#include "helpers.h"

#include <curses.h>

#include <cstdlib>

int initializeScreen() {
  initscr();
  if (!has_colors()) {
    return 1;  // no color no game!
  }
  clear();               // clear screen
  noecho();              // don't echo input
  raw();                 // handle input as raw
  curs_set(0);           // invisible cursor
  start_color();         // colors!
  keypad(stdscr, true);  // accept arrows
  return 0;
}

void closeScreen(int state) {
  clear();
  endwin();
  exit(state);
}
