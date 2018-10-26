// Minesweeper client skeleton.

// Created by:
// Jack (n)
// Corey (n10007164)

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "inc/includes.h"
#include "inc/networking.h"

char grid[NUM_TILES_X][NUM_TILES_Y]; // Client
char username[MAXDATASIZE];
char password[MAXDATASIZE];

#include "comms_client.c"
#include "inc/menus.h"

int flags = MINES;

/*
 * @brief handle Ctrl+C (SIGINT) terminations
 * @arg sig_num the signal receive (in this case, SIGINT)
 */
void sigint_handler(int sig_num) 
{ 
    signal(SIGINT, sigint_handler);
    disconnect_from_server();
    exit(1);
}

/*
* Flags the location provided by the parameters.
*/
void flag(int x, int y) {
     grid[y][x] = '+';

}

/*
 * @brief fill the grid with empty characters
 */
void reset_grid() {
     for (int y = 0; y < NUM_TILES_Y; y++) {
          for (int x = 0; x < NUM_TILES_X; x++) {
               grid[y][x] = ' ';
          }
     }

}


void gameProcess() {
     char coordinates[2] = "";
     int option1;
     bool quit = false;

     reset_grid();

     while (quit == false) {
          drawGame();
          option1 = drawOptionsPane();

          switch (option1) {
               case REVEAL_TILE:
                    printf("\nCoordinates: ");
                    scanf("%s", coordinates);
                    cmd_reveal_tile(coordinates);
                    break;
               case PLACE_FLAG:
                    printf("\nCoordinates: ");
                    scanf("%s", coordinates);
                    cmd_place_flag(coordinates);
                    break;
               case QUIT_GAME:
                    quit == true; // $$$ add returning to program process
                    break;
               default:
                    printf("An issue has occured processing your request. Please try again.");
                    break;
          }
     }
}

/*
* Program processes.
*/
void programProcess() {
     bool quit = false;
     char usernames[MAXENTRIES][MAXSTRINGSIZE];
     int seconds[MAXENTRIES];
     int gamesPlayed[MAXENTRIES];
     int gamesWon[MAXENTRIES];
     int output;
     while (quit == false) {
          int option1;
          option1 = drawMenu();
          switch (option1) {
               case PLAY_MINESWEEPER:
                    gameProcess();
                    break;
               case SHOW_LEADERBOARD:
                    output = generateLeaderboard(usernames, seconds, gamesPlayed, gamesWon);
                    if (output == CODE_SUCCESS) {
                         drawLeaderBoard(usernames, seconds, gamesPlayed, gamesWon);
                    }
                    else {
                         printf("\nThere was an error processing your request. Please try again later.\n");
                    }
                    break;
               case EXIT:
                    break;
               default:
                    printf("An issue has occured processing your request. Please try again.");
                    break;
          }
     }
}


/*
* Main.
*/
int main(int argc, char *argv[]) {
     if (argc != 3) {
          fprintf(stderr,"usage: client_hostname port_number\n");
          exit(1);
     }

     // Catch Ctrl+C and handle gracefully
     signal(SIGINT, sigint_handler); 

     sockfd = connect_to_server(argv[1], argv[2]);

     drawWelcomePane();


     bool AUTHENTICATED = false;

     while (!AUTHENTICATED) {
          requestUsername(username);
          requestPassword(password);

          if (authenticate(username, password) == CODE_ERROR) {
               loginUnsuccessful();
               continue;
          }
          AUTHENTICATED = true;
     }

     loginSuccessful();
     programProcess();

     disconnect_from_server();
     return 0;
}
