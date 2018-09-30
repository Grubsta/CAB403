// Minesweeper server skeleton.

// Created by:
// Jack (n)
// Corey (n10007164)

#include "inc/includes.h"



// Stores the surrounding 8 blocks at a point.
int grid[3][3] = {
  {-1, -1}, {0, -1}, {1, -1}
  {-1, 0},           {1, 0}
  {-1, 1},  {0, 1},  {1, 1}
};

/*
* Stores required variables of each tile.
*/
typedef struct {
  int adjacement_mines;
  bool visable;
  bool is_mine;
} Tile;

/*
* Stores required variables of a users game.
*/
typedef struct GameState {
  // Add more stored variable.
  int seconds; // Total seconds of current game.
  Tile tiles[NUM_TILES_X][NUM_TILES_Y]; // Stores the gamefield in an array
};

/*
* Stores required variables of a user.
*/
typedef struct User {
  char name;
  GameState game; // Curent game.
  int score; // Total score.
  int seconds; // Total seconds of playtime.
  int numGames; // Total number of games played
}

/*
* Recieves user input and calls the related function.
*/
void receiver() {

}

/*
* Sends the leaderboard to the user.
*/
void sendLeaderboard() {

}

/*
* Send the game process to the user.
*/
void sendProcess() {

}

/*
* Reads a text file and outputs the results to an array.
*/
int reader() {

}

/*
* Creates and initialises a game, and responds to the user.
*/
void createGame(User user) {
  // Initialising required structures.
  GameState game;
  user.game = game;

  // Initialising bombs at a seeded location.
  for (int i = 0; i < 10; i++) {
    x = srand(SEED) % NUM_TILES_X - 1;
    y = srand(SEED) % NUM_TILES_Y - 1;

    // Checking if tile is a bomb already.
    if (!(game.tiles[x][y].is_mine)) {
      game.tiles[x][y].is_mine = true;
    }
    else {
      i--; // Decrement 'i' to redo the loop.
    }
  }

  // Sets the integer value of surround bombs for each tile.
  for (int y = 0; y < NUM_TILES_Y; y++) {
    for (int x = 0; x < NUM_TILES_X; x++) {

      counter = 0; // Stores the surrounding mines.
      if (!(game.tiles[x][y].is_mine)) {
        // Checks surrounding 8 tiles for mines, and increments counter if 1 exists.
        for (a = 0; a < 3; a++) {
          for (b = 0; b < 3; b++) {
            if (game.tiles[a][b].is_mine) counter++;
          }
        }
      }
      game.tiles[x][y].adjacement_mines = counter; // Set adjacement mine variable to the counter
    }
  }
}

/*
* Processes a users move.
*/
void processMove(GameState gamestate) {

}

/*
* Shut downs server cleanly.
* Handles threads, open sockets, dynamically allocated memory and open files.
*/
void shutDown() {

}

/*
 * Server client communication connection test functionality.
 */
int *Receive_Array_Int_Data(int socket_identifier, int size) {
    int number_of_bytes, i=0;
    uint16_t statistics;

	int *results = malloc(sizeof(int)*size);
	for (i=0; i < size; i++) {
		if ((number_of_bytes=recv(socket_identifier, &statistics, sizeof(uint16_t), 0))
		         == RETURNED_ERROR) {
			perror("recv");
			exit(EXIT_FAILURE);

		}
		results[i] = ntohs(statistics);
	}
	return results;
}

int connect(int argc, char *argv[])
{
	int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	int i=0;

	/* Get port number for server to listen on */
	if (argc != 2) {
		fprintf(stderr,"usage: client port_number\n");
		return 1;
	}

	/* generate the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return 1;
	}

	/* generate the end point */
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(atoi(argv[1]));     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
		/* bzero(&(my_addr.sin_zero), 8);   ZJL*/     /* zero the rest of the struct */

	/* bind the socket to the end point */
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== -1) {
		perror("bind");
		return 1;
	}

	/* start listnening */
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		return 1;
	}

	printf("server starts listnening ...\n");

	/* repeat: accept, send, close the connection */
	/* for every accepted connection, use a sepetate process or thread to serve it */
	while(1) {  /* main accept() loop */
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
		&sin_size)) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));
		if (!fork()) { /* this is the child process */

			/* Call method to recieve array data */
			int *results = Receive_Array_Int_Data(new_fd,  ARRAY_SIZE);	

			/* Print out the array results sent by client */
			for (i=0; i < ARRAY_SIZE; i++) {
				printf("Value of index[%d] = %d\n", i, results[i]);
			}			

			free(results);

			if (send(new_fd, "All of array data received by server\n", 40 , 0) == -1)
				perror("send");
			close(new_fd);
			return 0;
		}
		close(new_fd);  /* parent doesn't need this */

		while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
	}
}

/*
* Main.
*/
int main(int argc, char *argv[]) {

  reader();

  int result = connect();
  exit(result);

}
