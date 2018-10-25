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
* Initialises the games playable field.
*/
void initField() {

}

/*
* Initialises the game by sending the server a request.
*/
void initGame() {

}

/*
* Outputs the in-game menu.
*/
void gameMenu() {

}

/*
* Outputs the leaderboard interface.
*/
void leaderBoard() {

}

/*
* Flags the location provided by the parameters.
*/
void flag(int x, int y) {
     grid[y][x] = '+';

}


/*
* Finishs the connections between user and server.
*/
void finish() {

}

/*
* Login functionality.
*/
void Login() {

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
                    break;
               case PLACE_FLAG:
                    scanf("%s", coordinates);
                    cmd_place_flag(sockfd, coordinates);
                    break;
               case QUIT_GAME:
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
     while (quit == false) {
          int option1;
          option1 = drawMenu();
          switch (option1) {
               case PLAY_MINESWEEPER:
                    gameProcess(sockfd);
                    break;
               case SHOW_LEADERBOARD:
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
