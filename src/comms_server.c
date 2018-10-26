#ifndef COMMS_SERVER_C
#define COMMS_SERVER_C
#include "inc/includes.h"
#include "inc/networking.h"

/*
 * @brief starts the listen server on the provided port
 * @arg port_number the port number to start the listen server on
 * @return the socket id to communicate over
*/
int start_listen_server(char * port_number) {
	int sockfd;                        /* listen on sock_fd, new connection on newfd */
	struct sockaddr_in server_addr;    /* my address information */
	struct sockaddr_in client_addr;    /* connector's address information */


     // Obtain the socket for communication with clients
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == CODE_ERROR) {
          perror("socket");
          exit(1);
     }

	server_addr.sin_family = AF_INET;                      /* host byte order */
	server_addr.sin_port = htons(atoi(port_number));       /* short, network byte order */
	server_addr.sin_addr.s_addr = INADDR_ANY;              /* auto-fill with my IP */

	// Bind the socket to the end point
     if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == CODE_ERROR) {
          perror("bind");
          exit(1);
     }

	// Start listening on the port
     if (listen(sockfd, 10) == CODE_ERROR) {
          perror("listen");
          exit(1);
     }

     // Returning the socket to communicate over
     return sockfd;
}

/*
 * @brief accept the connection from a client
 * @arg sockfd the socket id to accept connections from
 * @return the socket id to communicate with the client
*/
int connect_to_client(int sockfd) {
     int newfd;
     struct sockaddr_in client_addr;
	socklen_t sin_size;
     bool done = false;
     while(1) {
          sin_size = sizeof(struct sockaddr_in);
          // Repeatedly loop until a connection is accepted
          newfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
          if (newfd < 0) {
               continue;
          }

          break;
     }

     // Returning the socket id to communicate over
     return newfd;
}

/*
 * @brief reveal a tile at the specified coordinates and notify the client
 * @arg sockfd the socket to communicate over
 * @arg user the current user
 * @arg y the y coordinate to check
 * @arg x the x coordinate to check
 * @return -1 on fail, 0 on success
 */
int get_tile_value(User *user, int y, int x) {
     int count = user->game.tiles[y][x].adjacent_mines;
     if (count == 9) {
		user->numGames += 1;
		// add time to user
		// Game over, return back to menu
	}

     return count;
}

int reveal_grid(int sockfd, User* user) {
     int value;

     send_int(sockfd, BEGIN_GRID_REVEAL);

     if (receive_int(sockfd) != ACKNOWLEDGE_BEGIN_GRID_REVEAL) {
          printf("Error receiving acknowledgement of grid reveal\n");
     }

     for (int y = 0; y < NUM_TILES_Y; y++) {
          for (int x = 0; x < NUM_TILES_X; x++) {
               // Send Y coordinate
               send_int(sockfd, y);
               if (receive_int(sockfd) != y) {
                    printf("Error receiving confirmation of reception from client (Y coordinate %d)\n", y);
                    return CODE_ERROR;
               }

               // Send X coordinate
               send_int(sockfd, x);
               if (receive_int(sockfd) != x) {
                    printf("Error receiving confirmation of reception from client (X coordinate %d)\n", x);
                    return CODE_ERROR;
               }

               // Send value at Y and X coordinate
               value = user->game.tiles[y][x].adjacent_mines;

               send_int(sockfd, value);

               if (receive_int(sockfd) != value) {
                    printf("Error receiving confirmation of reception from client (Value %d)\n", value);
                    return CODE_ERROR;
               }
          }
     }

     send_int(sockfd, END_GRID_REVEAL);

     if (receive_int(sockfd) != ACKNOWLEDGE_END_GRID_REVEAL) {
          printf("Error receiving closing acknowledgement of grid reveal from client\n");
          return CODE_ERROR;
     }

     return CODE_SUCCESS;
}

/*
 * @brief process command protocol functions sent from the client
 * @arg sockfd the socket to communicate over
 * return -1 on fail, 0 on success
 */
int process_command(int sockfd, User* user) {
     int command_type;
     int y, x;
     send_int(sockfd, ACKNOWLEDGE_BEGIN_COMMAND);

     command_type = receive_int(sockfd);

     send_int(sockfd, command_type);

     switch (command_type) {
          case COMMAND_PLACE_FLAG:

               y = receive_int(sockfd);
               if (y < 0 || y > 8) {
                    printf("Error receiving Y coordinate (out of acceptable bounds)\n");
                    return CODE_ERROR;
               }

               x = receive_int(sockfd);
               if (x < 0 || x > 8) {
                    printf("Error receiving X coordinate (out of acceptable bounds)\n");
                    return CODE_ERROR;
               }


               // If flag doesn't exist in place
               if (!user->game.tiles[y][x].is_flag) {
                    // If mine is where flag is to be placed
                    if (user->game.tiles[y][x].is_mine) {
                         user->game.mines_left--;
                         user->game.flags_left--;
                         user->game.tiles[y][x].is_flag = true;
                         send_int(sockfd, COMMAND_PLACE_FLAG_SUCCESS);
                         printf("[SERVER] Flag placed. Mines remaining: %d. Flags remaining: %d\n", user->game.mines_left, user->game.flags_left);
                    }
                    else {
                         printf("[SERVER] Flag not placed.\n");
                         send_int(sockfd, COMMAND_PLACE_FLAG_FAIL);
                    }
               }
               else {
                    printf("[SERVER] Flag not placed.\n");
                    send_int(sockfd, COMMAND_PLACE_FLAG_FAIL);
               }

			if (user->game.flags_left <= 0) {
				if (user->game.mines_left <= 0) {
					user->gamesWon++;
                         send_int(sockfd, GAME_WON);

				}
				user->numGames++;
                    send_int(sockfd, GAME_OVER);
                    reveal_grid(sockfd, user);
				// go back to menu state
			}

               send_int(sockfd, END_COMMAND);
               if (receive_int(sockfd) != ACKNOWLEDGE_END_COMMAND) {
                         printf("Error receiving acknowledgement of command end from client\n");
                         return CODE_ERROR;
               }
               break;

          case COMMAND_REVEAL_TILE:
               y = receive_int(sockfd);
               if (y < 0 || y > 8) {
                    printf("Error receiving Y coordinate (out of acceptable bounds)\n");
                    return CODE_ERROR;
               }

               x = receive_int(sockfd);
               if (x < 0 || x > 8) {
                    printf("Error receiving X coordinate (out of acceptable bounds)\n");
                    return CODE_ERROR;
               }

               int tile = get_tile_value(user, y, x);
               send_int(sockfd, tile);

               if (tile == 9) {
                    send_int(sockfd, GAME_OVER);
                    user->numGames++;
               }
               else {
                    send_int(sockfd, END_COMMAND);
                    if (receive_int(sockfd) != ACKNOWLEDGE_END_COMMAND) {
                         printf("Error receiving acknowledgement of command end from client\n");
                         return CODE_ERROR;
                    }
               }
               break;

          default:
               break;
     }
     return CODE_SUCCESS;
}

/*
 * @brief send the leaderboard to the client on when requested
 * @arg sockfd the socket to communicate over
 * @arg user the currently logged in user
 * @return -1 on failure, 0 on success
 */
int transmit_leaderboard(int sockfd, User user) {
     // char username = user.username;
     //send_char(sockfd, username);
     //strcpy(username, &user->username);

     send_int(sockfd, ACKNOWLEDGE_BEGIN_TRANSMIT_LEADERBOARD);

     send_int(sockfd, BEGIN_TRANSMIT_LEADERBOARD_ENTRY);

     if (receive_int(sockfd) != ACKNOWLEDGE_BEGIN_LEADERBOARD_ENTRY) {
          printf("Error receiving acknowledgement of leaderboard entry\n");
          return CODE_ERROR;
     }

     if (send_char(sockfd, user.username) != CODE_SUCCESS) {
          printf("Error sending username of leaderboard entry\n");
          return CODE_ERROR;
     }

     send_int(sockfd, user.seconds);//user->seconds);
     send_int(sockfd, user.numGames);//user->numGames);
     send_int(sockfd, user.gamesWon);//user->gamesWon);

     send_int(sockfd, END_TRANSMIT_LEADERBOARD_ENTRY);

     if (receive_int(sockfd) != ACKNOWLEDGE_END_LEADERBOARD_ENTRY) {
          printf("Error receiving acknowledgement of end of leaderboard entry\n");
          return CODE_ERROR;
     }

     send_int(sockfd, END_TRANSMIT_LEADERBOARD);

     if (receive_int(sockfd) != ACKNOWLEDGE_END_TRANSMIT_LEADERBOARD) {
          printf("Error receiving acknowledgement of end of leaderboard\n");
          return CODE_ERROR;
     }

     return CODE_SUCCESS;
}

/*
 * @brief receive username and password from client to authenticate connection
 * @arg sockfd the socket id to receive credentials via
 * @return -1 on failure, 0 on success
 */
int authenticate(int sockfd, char usernames[][MAXSTRINGSIZE], char passwords[][MAXSTRINGSIZE], int sizeArray, User* user) {
     char username[MAXDATASIZE];
     char password[MAXDATASIZE];

     send_int(sockfd, ACKNOWLEDGE_BEGIN_AUTHENTICATE);

     if (receive_int(sockfd) != BEGIN_TRANSMIT_STRING) {
          printf("Error receiving begin string transmit code\n");
          return CODE_ERROR;
     }

     if ((receive_char(sockfd, username)) != CODE_SUCCESS) {
          printf("Error receiving username from client\n");
          return CODE_ERROR;
     }

     send_int(sockfd, ACKNOWLEDGE_RECEIVED_USERNAME);

     if (receive_int(sockfd) != BEGIN_TRANSMIT_STRING) {
          printf("Error receiving begin string transmit code\n");
          return CODE_ERROR;
     }

     if (receive_char(sockfd, password) != CODE_SUCCESS) {
          printf("Error receiving password from client\n");
          return CODE_ERROR;
     }

     send_int(sockfd, ACKNOWLEDGE_RECEIVED_PASSWORD);

	for (int i = 2; i <= sizeArray; i++){
		if ((strcmp(username, usernames[i]) == 0) && (strcmp(password, passwords[i]) == 0)) {
			send_int(sockfd, AUTHENTICATE_SUCCESS);
			strcpy(user->username, usernames[i]);
			strcpy(user->password, passwords[i]);
			return CODE_SUCCESS;
		}
	}
     send_int(sockfd, AUTHENTICATE_FAILURE);
     return CODE_ERROR;

}

#endif
