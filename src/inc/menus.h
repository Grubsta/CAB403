#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int active_mines;

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
		printf("%c\n", option);
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
    if (option == 'R') return REVEALTILE;
		if (option == 'P') return PLACEFLAG;
		if (option == 'Q') return QUITGAME;
    printf("\n\nThe selection was not valid. Please try again\n");
  }
}



void drawGame(){
	printf("\n\n\nMines Remaining: %d\n", active_mines);
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

// Draws leaderboards dependent on amount of users. No users will result in an empty table
void drawLeaderBoard(){
	// Empty Leaderboard scenario
	printf("\n========================================================================\n");
	printf("USER\t\tHIGHSCORE\t\tTIME\t\tGAMES");
  // for (all results)
  // printf("%s\t\t%d\t\t%d\t\t%d\n", user[i])
	printf("\n========================================================================\n\n");

}
