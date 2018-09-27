// Minesweeper server skeleton.

#include "includes.h"

/*
*
*/
typedef struct {
  int adjacement_mines;
  bool visable;
  bool is_mine;
} Tile;

/*
* Stores required variables of a users game.
*/
typedef struct GameState {
  // Add more stored variables
  Tile tiles [NUM_TILES_X][NUM_TILES_Y];
};

/*
* Reads a text file and outputs the results to an array.
*/
int reader() {

}

/*
* Shut downs server cleanly.
* Handles threads, open sockets, dynamically allocated memory and open files.
*/
void shutDown() {

}

/*
* Main.
*/
int main(int argc, char *argv[]) {
  srand(SEED);
  reader();

}
