#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char username, password;
int active_mines;

void requestUsername(){
	printf("Username: ");
	scanf("%s", &username);
}

void requestPassword(){
	printf("\nPassword: ");
	scanf("%s", &password);

}
void drawWelcomePane(){
	printf("======================================================== \n");
	printf("Welcome to the online version of Minesweeper \n");
	printf("======================================================== \n");
	printf("\nYou are required to log on with your registered user name and password. \n");
  requestUsername();
  requestPassword();
}

void drawIncorrectLogin(){
	printf("You entered either an incorrect username or password. Disconnecting.\n");

}


int drawMenu(){
	printf("\nPlease select an option below: \n");
	printf("<1> Play Minesweeper \n");
	printf("<2> Show Leaderboard \n");
	printf("<3> Exit \n");
  int valid = 0;
  int option;
  while (valid == 0){
    printf("Selection (1-3): ");
    scanf("%d", &option);
    if (option >= 1 && option <= 3) {
      return option;
    }
    printf("\n\nThe selection was not valid. Please try again\n\n");
  }

}

void drawOptionsPane(){
	printf("\n\n\nChoose an option: \n");
	printf("<R> Reveal Tile\n");
	printf("<P> Place Flag\n");
	printf("<Q> Quit Game\n");
	printf("\nOption (R,P,Q): ");

}

void drawGame(){
	printf("\n\n\nMines Remaining: %d\n", active_mines);
	printf("\n    1 2 3 4 5 6 7 8 9 \n");
	printf("----------------------\n");
	for (int x = 0; x < NUM_TILES_X; x++) {
		printf("%c | ", x_Axis[x]);
		for (int y = 0; y < NUM_TILES_Y; y++) {
			printf("%c ", grid[x][y]);
		}
		printf("\n");
	}
}

void drawZeros(Grid tempGrid){
	printf("\n\n\nMines Remaining: %d\n", active_mines);
	printf("\n    1 2 3 4 5 6 7 8 9 \n");
	printf("----------------------\n");
	for (int x = 0; x < NUM_TILES_X; x++) {
		printf("%c | ", x_Axis[x]);
		for (int y = 0; y < NUM_TILES_Y; y++) {
			printf("%d ", tempGrid.grid[x][y]);
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
