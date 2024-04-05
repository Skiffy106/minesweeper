#ifndef helpers
#define helpers

int initializeScreen();
int closeScreen(int state);
int handleMenu();
void playGame(int mode, int width, int height);
int** generateGame(int mode, int width, int height);
int getNeighbors(int row, int col, int **data);

#endif