#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void loginSuccessful(){
	printf("\nLogin Successful!\n");
}

void loginUnsuccessful(){
	printf("\nThe Username or Password you entered was incorrect. Please try again\n");
}

void requestUsername(char * destination){
    printf("\nUsername: ");
    scanf("%s", destination);
}

void requestPassword(char * destination){
    printf("\nPassword: ");
    scanf("%s", destination);
}

void requestCoordinates(int destination[2]) {
     int y;
     int x;
     int i;
     char coordinates[512];

     while (1) {
          y = -1;
          x = -1;

          printf("\nCoordinates (e.g. C5): ");
          scanf("%s", coordinates);

          if (coordinates[0] == '\0' || coordinates[1] == '\0' || coordinates[2] != '\0') {
               printf("Please enter 2 characters.\n");
               continue;
          }
     
          for (i = 0; i < NUM_TILES_Y; i++) {
               if (coordinates[0] == y_Axis[i]) {
                    y = i;
                    break;
               }
          }
     
          for (i = 0; i < NUM_TILES_X; i++) {
               if (coordinates[1] == x_Axis[i]) {
                    x = i;
                    break;
               }
          }

          if (x == -1 || y == -1) {
               printf("Invalid input. Please try again.\n");
               continue;
          }
          break;
     }

     destination[0] = y;
     destination[1] = x;
}


void drawWelcomePane(){
	printf("======================================================== \n");
	printf("Welcome to the online version of Minesweeper \n");
	printf("======================================================== \n");
	printf("\nYou are required to log on with your registered user name and password. \n");
}

void drawIncorrectLogin(){
	printf("You entered either an incorrect username or password. Disconnecting.\n");

}

int drawMenu(){
	printf("\nPlease select an option below: \n");
	char option;
  while (true){
		scanf("%c", &option);
		printf("<1> Play Minesweeper \n");
		printf("<2> Show Leaderboard \n");
		printf("<3> Exit \n");

    printf("Selection (1-3): ");
    scanf("%c", &option);
    if (option == '1' || option == '2' || option == '3') {
			int intOption = option - '0';
      return intOption;
    }
    printf("\n\nThe selection was not valid. Please try again\n\n");
  }
}

int drawOptionsPane(){
	char option;
	printf("\n\n\n");
	while (true) {
		scanf("%c", &option);
		printf("\nChoose an option: \n");
		printf("<R> Reveal Tile\n");
		printf("<P> Place Flag\n");
		printf("<Q> Quit Game\n");
		printf("\nOption (R,P,Q): ");
    scanf("%c", &option);
    if (option == 'R') return REVEAL_TILE;
		if (option == 'P') return PLACE_FLAG;
		if (option == 'Q') return QUIT_GAME;
    printf("\n\nThe selection was not valid. Please try again\n");
  }
}



void drawGame(){
	printf("\n\n\nMines Remaining: %d\n", flags_remaining);
	printf("\n    1 2 3 4 5 6 7 8 9 \n");
	printf("----------------------\n");
	for (int y = 0; y < NUM_TILES_Y; y++) {
		printf("%c | ", y_Axis[y]);
		for (int x = 0; x < NUM_TILES_X; x++) {
			printf("%c ", grid[y][x]);
		}
		printf("\n");
	}
}

void drawGameOver(){
	printf("\n\n Oh No! Looks like you hit a mine :(\n");
}

void drawGameWon() {
     printf("\n\nWoohoo! You win! :)\n");
}

// Draws leaderboards dependent on amount of users. No users will result in an empty table
void drawLeaderBoard(char * username, int results[3]){
	// Empty Leaderboard scenario
	printf("\n========================================================================\n");
	printf("%s\t\t%d seconds\t\t%d games won, %d games played\n", username, results[0], results[1], results[2]);
  // printf("%s\t\t%d\t\t%d\t\t%d\n", user[i])
	printf("\n========================================================================\n\n");

}

#endif
