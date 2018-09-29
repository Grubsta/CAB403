// Minesweeper server skeleton.

// Created by:
// Jack (n)
// Corey (n10007164)

#include "inc/includes.h"



// Stores the surrounding 8 blocks at a point.
int grid[3][3] = {
  {-1, -1}, {0, -1}, {1, -1}
  {-1, 0},           {1, 0}
  {-1, 1},  {0, 1},  {1, 1}
};

/*
* Stores required variables of each tile.
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
  // Add more stored variable.
  int seconds; // Total seconds of current game.
  Tile tiles[NUM_TILES_X][NUM_TILES_Y]; // Stores the gamefield in an array
};

/*
* Stores required variables of a user.
*/
typedef struct User {
  char name;
  GameState game; // Curent game.
  int score; // Total score.
  int seconds; // Total seconds of playtime.
  int numGames; // Total number of games played
}

/*
* Recieves user input and calls the related function.
*/
void receiver() {

}

/*
* Sends the leaderboard to the user.
*/
void sendLeaderboard() {

}

/*
* Send the game process to the user.
*/
void sendProcess() {

}

/*
* Reads a text file and outputs the results to an array.
*/
int reader() {

}

/*
* Creates and initialises a game, and responds to the user.
*/
void createGame(User user) {
  // Initialising required structures.
  GameState game;
  user.game = game;

  // Initialising bombs at a seeded location.
  for (int i = 0; i < 10; i++) {
    x = srand(SEED) % NUM_TILES_X - 1;
    y = srand(SEED) % NUM_TILES_Y - 1;

    // Checking if tile is a bomb already.
    if (!(game.tiles[x][y].is_mine)) {
      game.tiles[x][y].is_mine = true;
    }
    else {
      i--; // Decrement 'i' to redo the loop.
    }
  }

  // Sets the integer value of surround bombs for each tile.
  for (int y = 0; y < NUM_TILES_Y; y++) {
    for (int x = 0; x < NUM_TILES_X; x++) {

      counter = 0; // Stores the surrounding mines.
      if (!(game.tiles[x][y].is_mine)) {
        // Checks surrounding 8 tiles for mines, and increments counter if 1 exists.
        for (a = 0; a < 3; a++) {
          for (b = 0; b < 3; b++) {
            if (game.tiles[a][b].is_mine) counter++;
          }
        }
      }
      game.tiles[x][y].adjacement_mines = counter; // Set adjacement mine variable to the counter
    }
  }
}

/*
* Processes a users move.
*/
void processMove(GameState gamestate) {

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

  reader();

}
