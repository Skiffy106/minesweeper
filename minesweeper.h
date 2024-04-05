#ifndef minesweeper
#define minesweeper

#include <vector>

class Minesweeper
{
private:
    std::vector <std::vector <char> > board;
    int height, width, mines, game_height, game_width;
    void generateGame();
    int getNeighbors(int row, int col);
    void showBoard();
public:
    Minesweeper();
    ~Minesweeper();
    void handleMenu();
    void playGame();
};


#endif