#ifndef minesweeper
#define minesweeper

#include <vector>

class Minesweeper {
 private:
  std::vector<std::vector<char> > board;
  std::vector<std::vector<bool> > shown;
  int height, width, mines, game_height, game_width, cursorX, cursorY;
  void generateGame();
  int getNeighbors(int row, int col);
  void showBoard();
  void showCursor();
  void initClass();
  void showLose();
  void showWin();
  bool isWon();
  void floodFill(int x, int y);

 public:
  Minesweeper();
  ~Minesweeper();
  void handleMenu();
  void playGame();
};

#endif