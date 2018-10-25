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
#include "comms_client.c"

char grid[NUM_TILES_X][NUM_TILES_Y]; // Client
char x_Axis[NUM_TILES_X] = "ABCDEFGHI"; // Client.


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


/*
* Game processes.
*/
void gameProcess() {
  bool quit = false;
  while (quit == false) {
    int option1;
    option1 = drawOptionsPane();
    switch (option1) {
      case 1:
        break;
      case 2:
        break;
      case 3:
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

     char username[MAXDATASIZE];
     char password[MAXDATASIZE];

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
     gameProcess();

     disconnect_from_server(sockfd);
     return 0;
}
