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
* Game processes.
*/
void gameProcess() {

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
* Main.
*/
int main(int argc, char *argv[]) {
    drawWelcomePane();
    requestUsername();
    requestPassword();
    loginSuccessful();
    drawMenu();
    // exit(result);
}
