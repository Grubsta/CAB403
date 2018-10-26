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
 * @brief fill the grid with empty characters
 */
void reset_grid() {
     for (int y = 0; y < NUM_TILES_Y; y++) {
          for (int x = 0; x < NUM_TILES_X; x++) {
               grid[y][x] = ' ';
          }
     }

}

/*
 * @brief the looping process to run through the game on client side (gameloop)
 * @return -1 on failure, 0 on success
 */ 
int gameProcess() {
     int option1;
     bool quit = false;
     int coordinates[2];
     int result;

     reset_grid();

     while (quit == false) {
          drawGame();
          option1 = drawOptionsPane();

          switch (option1) {
               case REVEAL_TILE:
                    requestCoordinates(coordinates);
                    result = cmd_reveal_tile(coordinates[0], coordinates[1]);
                    printf("%d\n", result);
                    switch (result) {
                         case GAME_OVER:
                              drawGameOver();
                              quit = true;
                              break;

                         case GAME_WON:
                              drawGameWon();
                              quit = true;
                              break;
                    }
                    break;

               case PLACE_FLAG:
                    requestCoordinates(coordinates);
                    result = cmd_place_flag(coordinates[0], coordinates[1]);
                    switch (result) {
                         case GAME_OVER:
                              drawGameOver();
                              quit = true;
                              break;

                         case GAME_WON:
                              drawGameWon();
                              quit = true;
                              break;
                    }
                    break;

               case QUIT_GAME:
                    quit = true; // $$$ add returning to program process
                    break;
               default:
                    printf("An issue has occured processing your request. Please try again.\n");
                    break;
          }
     }

     send_int(sockfd, END_GAME);
     if (receive_int(sockfd) != ACKNOWLEDGE_END_GAME) {
          printf("Error receiving end game acknowledgement from server\n");
          return CODE_ERROR;
     }

     reveal_grid();
     drawGame();

     return CODE_SUCCESS;
}

/*
 * @brief the program process including background functionality and menu selections
 */
void programProcess() {
     bool quit = false;
     char usernames[MAXENTRIES][MAXSTRINGSIZE];
     int seconds[MAXENTRIES];
     int gamesPlayed[MAXENTRIES];
     int gamesWon[MAXENTRIES];
     int output;
     char username[MAXDATASIZE];
     int results[3];
     while (quit == false) {
          int option1;
          option1 = drawMenu();
          switch (option1) {
               case PLAY_MINESWEEPER:
                    send_int(sockfd, BEGIN_GAME);
                    gameProcess();
                    break;
               case SHOW_LEADERBOARD:
                    if (generateLeaderboard(username, results) != CODE_SUCCESS) {
                         printf("\nThere was an error processing your request. Please try again later.\n");
                    }
                    else {
                         drawLeaderBoard(username, results);
                    }
                    break;
               case EXIT:
                    disconnect_from_server();
                    quit = true;
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
