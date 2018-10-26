// Minesweeper server skeleton.

// Created by:
// Jack (n)
// Corey (n10007164)

#include "time.h"
#include "inc/includes.h"
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

/*
* Stores a 2d array of tiles
*/
typedef struct Grid {
  int grid[NUM_TILES_Y][NUM_TILES_X];
} Grid;

/*
* Stores required variables of a users game.
*/
typedef struct GameState {
  // Add more stored variable.
  int seconds; // Total seconds of current game.
  Tile tiles[NUM_TILES_Y][NUM_TILES_X]; // Stores the gamefield in an array
  int flags_left;
  int mines_left;
  clock_t gameClock;
} GameState;

/*
* Stores required variables of a user.
*/
typedef struct User {
  char username;
  char password;
  GameState game; // Curent game.
  int seconds; // Total seconds of playtime.
  int numGames; // Total number of games played
  int gamesWon;
} User;


#include "comms_server.c"
#include "authentication.c"


/*
* @brief Creates and initialises a game, and returns Gamestate structure.
*/
GameState createGame() {
  // Create new game structure.
  GameState game;

  // Seed random function.
  srand(SEED);

  // Set default values.
  game.seconds = 0;
  game.mines_left = MINES;
  game.flags_left = FLAGS;
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
  // Loop over game field allocating numbers which represent the amount of adjacent mines
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
* @brief Recursively check neighbouring values and output a 3x3 array of 1's where the value
* on the game grid is 0. Note: it is impossible for a mine to be shown in this step.
* @param X - x position on GameState
* @param Y - y position on GameState
* @param *game - pointer to GameState which is being examined.
* @return Return a grid structure where any values revealed will be associated with adjacent mines,
* else the position will be '0' or NULL;
*/
Grid checkNeighbours(int X, int Y, GameState *game) {
     Grid tempGrid;
     // Default all values to 10 - an unreachable number in minesweeper
     for (int d = 0; d < NUM_TILES_Y; d++) {
          for (int c = 0; c < NUM_TILES_X; c++) {
               tempGrid.grid[d][c] = '0';
          }
     }

     // Set tile which have been checked to true to prevent never ending loops
     game->tiles[X][Y].is_checked = true;
     tempGrid.grid[X][Y] = game->tiles[X][Y].adjacent_mines;

     // Loop through neighbouring blocks checking for a zero
     for (int b = Y - 1; b <= Y + 1; b++) {
          for (int a = X - 1; a <= X + 1; a++) {

               // Ensure value being examined is legal (within grid)
               bool insideGrid = true;
               if (a < 0 || a >= NUM_TILES_X || b < 0 || b >= NUM_TILES_Y) insideGrid = false;

               // Ensure value is legal and hasn't aleady been checked
               if (insideGrid == true && !(game->tiles[b][a].is_checked)) {

                    game->tiles[b][a].is_checked = true;

                    // Add point to grid
                    tempGrid.grid[b][a] = game->tiles[b][a].adjacent_mines;

                    // Ensure the value has no surround bombs
                    if (game->tiles[b][a].adjacent_mines == 0) {

                          Grid outputtedGrid;
                          outputtedGrid = checkNeighbours(b, a, game);

                          // Loop through entire grid, adding values where not null
                          for (int d = 0; d < NUM_TILES_Y; d++) {
                               for (int c = 0; c < NUM_TILES_X; c++) {

                                   // Check that value in grid is not null
                                   if (outputtedGrid.grid[d][c] != '0') {

                                        // Add value in the grid if it is not already added
                                        if (tempGrid.grid[d][c] == '0') {
                                             tempGrid.grid[d][c] = game->tiles[d][c].adjacent_mines;
                                        }
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
* Main.
*/
int main(int argc, char *argv[]) {
	/* Get port number for server to listen on */
	if (argc != 2) {
		fprintf(stderr,"usage: client port_number\n");
		exit(1);
	}

     // Generate 2 arrays of usernames and passwords
     int userCount = countUsers();
     char usernames[userCount][MAXSTRINGSIZE];
     char passwords[userCount][MAXSTRINGSIZE];
     int output = GenerateUsers(usernames, passwords);

     int sockfd = start_listen_server(argv[1]);
     printf("[SERVER] Listen server started on port %s.\n", argv[1]);

     int newfd = connect_to_client(sockfd);
     printf("[SERVER] Established connection to client.\n");

     // Instruction loop
     bool GAME_STARTED = false;
     bool AUTHENTICATED = false;
     bool PROGRAM_END = false;
     int instruction, instruction_old;

     User user;
     user.seconds = 0;
     GameState game;

     while (!PROGRAM_END) {
          // Recieve the latest instruction
          instruction = receive_int(newfd);

          // Ignore if the latest instruction is the same
          if (instruction == instruction_old) {
               continue;
          }

          // If the client is not authenticated, attempt to authenticate them
          if (!AUTHENTICATED) {
               if (authenticate(newfd, usernames, passwords, userCount, user) == CODE_ERROR) {
                    printf("[SERVER] User failed to authenticate\n");
                    continue;
               }

               AUTHENTICATED = true;
               printf("[SERVER] User authenticated successfully\n");
          }

          if (!GAME_STARTED) {
               switch(instruction) {
                    case BEGIN_GAME:
                         game = createGame();
                         user.game = game;
                         user.game.gameClock = clock();

                         GAME_STARTED = true;
                         user.numGames++;
                         printf("[SERVER] New game started by client\n");
                         break;
                    default:
                         break;
               }
          }

          // Start accepting instructions
          switch (instruction) {
               case END_CONNECTION:
                    PROGRAM_END = true;
                    printf("[SERVER] Connection to client closed\n");
                    break;
               case END_GAME:
                    send_int(newfd, ACKNOWLEDGE_END_GAME);
                    reveal_grid(newfd, &user);
                    GAME_STARTED = false;

                    // Handle clock functionality
                    clock_t done = clock();
                    printf("[SERVER] Time Game Started: %ld\t\t", user.game.gameClock);
                    printf("Time Game Completed: %ld\n",done );
                    clock_t diff = (double)(done - user.game.gameClock) / 100;
                    user.seconds += (int) diff;
                    printf("TIME: %d\n",user.seconds);

                    printf("[SERVER] Current game ended\n");
                    break;
               case BEGIN_TRANSMIT_LEADERBOARD:
                    transmit_leaderboard(newfd, &user);
                    break;
               case BEGIN_COMMAND:
                    process_command(newfd, &user);
                    break;
               default:
                    break;
          }
     }

     close(newfd);
     close(sockfd);
     return 0;
}
