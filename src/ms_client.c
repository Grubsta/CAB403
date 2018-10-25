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
#include "comms_client.c"

char grid[NUM_TILES_X][NUM_TILES_Y]; // Client
char username[MAXDATASIZE];
char password[MAXDATASIZE];

#include "inc/menus.h"

int flags = MINES;


/*
* Flags the location provided by the parameters.
*/
void flag(int x, int y) {
     grid[y][x] = '+';

}


void gameProcess(int sockfd) {
     char coordinates[2] = "";
     int option1;
     bool quit = false;

     while (quit == false) {
          drawGame();
          option1 = drawOptionsPane();

          switch (option1) {
               case REVEAL_TILE:
                    printf("\nCoordinates: ");
                    scanf("%s", coordinates);
                    cmd_reveal_tile(sockfd, coordinates);
                    break;
               case PLACE_FLAG:
                    printf("\nCoordinates: ");
                    scanf("%s", coordinates);
                    cmd_place_flag(sockfd, coordinates);
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
void programProcess(int sockfd) {
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
                    gameProcess(sockfd);
                    break;
               case SHOW_LEADERBOARD:
                    output = generateLeaderboard(sockfd, usernames, seconds, gamesPlayed, gamesWon);
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

     int sockfd = connect_to_server(argv[1], argv[2]);

     drawWelcomePane();


     bool AUTHENTICATED = false;

     while (!AUTHENTICATED) {
          requestUsername(username);
          requestPassword(password);

          if (authenticate(sockfd, username, password) == CODE_ERROR) {
               loginUnsuccessful();
               continue;
          }
          AUTHENTICATED = true;
     }

     loginSuccessful();
     programProcess(sockfd);

     disconnect_from_server(sockfd);
     return 0;
}
