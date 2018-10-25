// Minesweeper server skeleton.

// Created by:
// Jack (n)
// Corey (n10007164)

#include "inc/includes.h"
#include "comms_server.c"
#include "authentication.c"



// Stores the surrounding 8 blocks at a point.

/*
* Stores required variables of each tile.
*/
typedef struct {
  int adjacent_mines;
  bool visable;
  bool is_mine;
  bool is_flag;
  bool is_checked;
} Tile;

typedef struct Grid {
  int grid[NUM_TILES_Y][NUM_TILES_X];
} Grid; // SERVER

/*
* Stores required variables of a users game.
*/
typedef struct GameState {
  // Add more stored variable.
  int seconds; // Total seconds of current game.
  Tile tiles[NUM_TILES_Y][NUM_TILES_X]; // Stores the gamefield in an array
  int mines;
} GameState;

/*
* Stores required variables of a user.
*/
typedef struct User {
  char username;
  char password;
  GameState game; // Curent game.
  int score; // Total score.
  int seconds; // Total seconds of playtime.
  int numGames; // Total number of games played
  int gamesWon;
} User;

typedef struct userGrid {
  char gridChar[NUM_TILES_Y][NUM_TILES_X];
} userGrid;



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
* Updates inputted XY position to
*/
// SERVER
void handleFlag(int X, int Y, GameState game) {
  game.tiles[Y][X].is_flag = true;
  if (game.tiles[Y][X].is_mine) {
    game.mines--;
  }
}

/*
* Creates and initialises a game, and responds to the user.
*/
GameState createGame() {
  // Initialising required structures.
  GameState game;
  // user.game = game;
  srand(SEED);

  // Set default values
  game.seconds = 0;
  game.mines = 10;
  for (int x = 0; x < NUM_TILES_X; x++) {
    for (int y = 0; y < NUM_TILES_Y; y++) {
      game.tiles[y][x].visable = false;
      game.tiles[y][x].is_mine = false;
      game.tiles[y][x].is_flag = false;
      game.tiles[y][x].is_checked = false;
    }
  }
  // Initialising bombs at a random location.
  for (int i = 0; i < MINES; i++) {
    int x = rand() % NUM_TILES_X;
    int y = rand() % NUM_TILES_Y;

    // Checking if tile is a bomb already.
    if (!(game.tiles[y][x].is_mine)) {
      game.tiles[y][x].is_mine = true;
      game.tiles[y][x].adjacent_mines = 9;
    }
    else {
      i--; // Decrement 'i' to redo the loop.
    }
  }
  // Loop over game field allocating numbers which represent the amount
  for (int y = 0; y < NUM_TILES_Y; y++) {
    for (int x = 0; x < NUM_TILES_X; x++) {

      int counter = 0; // Stores the surrounding mines.
      if (!(game.tiles[y][x].is_mine)) {
        // Checks surrounding 8 tiles for mines, and increments counter if 1 exists.
        for (int b = y - 1; b <= y + 1; b++) {
          // Ensures value is not outside of array
          if (b >= 0 && b < NUM_TILES_X) {
              for (int a = x - 1; a <= x + 1; a++) {
                if (a >= 0 && a < NUM_TILES_X) {

                if (game.tiles[b][a].is_mine) {
                  counter++;
                }
              }
            }
          }
        }
        game.tiles[y][x].adjacent_mines = counter; // Set adjacent mine variable to the counter
      }
    }
  }
  return game;
}

/*
* Recursively check neighbouring values and output a 3x3 array of 1's where the value
* on the game grid is 0.
* Note: it is impossible for a mine to be shown in this step.
*/
Grid checkNeighbours(int X, int Y, GameState game) {
  Grid tempGrid;
  Grid outputtedGrid;
  // Default all values to 10 - an unreachable number in minesweeper
  for (int d = 0; d < NUM_TILES_Y; d++) {
    for (int c = 0; c < NUM_TILES_X; c++) {
        tempGrid.grid[d][c] = 10;
    }
  }

  // Set tiles which have been checked to true to prevent never ending loops
  game.tiles[X][Y].is_checked = true;
  // Loop through neighbouring blocks cheching for a zero
  for (int b = Y - 1; b <= Y + 1; b++) {
    for (int a = X - 1; a <= X + 1; a++) {
      game.tiles[b][a].is_checked = true;

      // Ensure value being examined is legal
      bool insideGrid = true;
      if (X < 0 || X >= NUM_TILES_X || Y < 0 || Y >= NUM_TILES_Y) insideGrid = false;

      // If value is 0, repeat process
      if (game.tiles[b][a].adjacent_mines == 0 && insideGrid == true) {

        // Add all neighbouring cells to the grid
        for (int d = Y - 1; d <= Y + 1; d++) {
          // Check the value is not outside of grid
          if (!(d < 0 || d >= NUM_TILES_Y)) {
            for (int c = X - 1; c <= X + 1; c++) {
              if (!(c < 0 || c >= NUM_TILES_X)) {

                // Add point to grid
                tempGrid.grid[d][c] = game.tiles[d][c].adjacent_mines;

                // Recursively call function for all 0's in surrounding grid
                if ((!game.tiles[d][c].is_checked) &&  game.tiles[d][c].adjacent_mines == 0) {
                  game.tiles[d][c].is_checked == true;
                  printf("%d, %d\n", d, c);
                  outputtedGrid = checkNeighbours(d, c, game);
                }
                game.tiles[d][c].is_checked == true;
              }
            }
          }
        }

        // Loop through entire grid, adding values where not null
        for (int d = 0; d < NUM_TILES_Y; d++) {
          for (int c = 0; c < NUM_TILES_X; c++) {
            // Check that value in grid is not null
            if (outputtedGrid.grid[c][d] != 10) {
              // Add value in the grid if it is not already added
              if (game.tiles[d][c].is_checked && tempGrid.grid[d][c] == 10) {
                tempGrid.grid[d][c] = game.tiles[d][c].adjacent_mines;
              }
            }
          }
        }
      }
    }
  }
  return tempGrid;
}

/*
* Processes a users move.
*/
void processMove(GameState gamestate) {

}

/*
* Shut downs server cleanly.
* Handles threads, open sockets, dynamically allocated memory.
*/
void shutDown() {

}

/*
* Main.
*/
int main(int argc, char *argv[]) {
	/* Get port number for server to listen on */
	if (argc != 2) {
		fprintf(stderr,"usage: client port_number\n");
		exit(1);
	}

     // Generate 2 arrays of usernames and passwords
     int maxSize = 20;
     int userCount = countUsers();
     char usernames[userCount][maxSize];
     char passwords[userCount][maxSize];
     int output = GenerateUsers(usernames, passwords);
     printf("%s\n", passwords[1]);

     int sockfd = start_listen_server(argv[1]);
     printf("[SERVER] Listen server started on port %s.\n", argv[1]);

     int newfd = connect_to_client(sockfd);
     printf("[SERVER] Established connection to client.\n");

     // Instruction loop
     bool AUTHENTICATED = false;
     bool GAME_END = false;
     int instruction, instruction_old;

     while (!GAME_END) {
          // Recieve the latest instruction
          instruction = receive_int(newfd);

          // Ignore if the latest instruction is the same
          if (instruction == instruction_old) {
               continue;
          }

          // If the client is not authenticated, attempt to authenticate them
          if (!AUTHENTICATED) {
               if (authenticate(newfd) == CODE_ERROR) {
                    printf("Error authenticating\n");
                    continue;
               }

               AUTHENTICATED = true;
          }

          // Start accepting instructions
          switch (instruction) {
               case END_CONNECTION:
                    GAME_END = true;
          }
     }

     close(newfd);
     close(sockfd);
     return 0;
}
