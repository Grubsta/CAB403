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
int process_command_reveal_tile(int sockfd, User user, int y, int x) {
     /*int count = user.game.tiles[y][x].adjacent_mines;
     printf("Tile: %d\n", count);*/
     int count = 1;

     send_int(sockfd, count);
}

/*
 * @brief process command protocol functions sent from the client
 * @arg sockfd the socket to communicate over
 * return -1 on fail, 0 on success
 */
int process_command(int sockfd, User user) {
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

               /*if (process_command_place_flag(y, x) != CODE_SUCCESS) {
                    printf("Error placing flag");
                    return CODE_ERROR;
               }*/

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

               if (process_command_reveal_tile(sockfd, user, y, x) != CODE_SUCCESS) {
                    printf("Error revealing tile");
                    return CODE_ERROR;
               }

               break;

          default:
               break;
     }

     send_int(sockfd, END_COMMAND);

     return CODE_SUCCESS;
}

int transmit_leaderboard(int sockfd, int sizeArray, GameState game) {

	if (receive_int(sockfd) != BEGIN_TRANSMIT_LEADERBOARD) {
		printf("Error receiving request for leaderboard\n");
		return CODE_ERROR;
	}

	send_int(sockfd, ACKNOWLEDGE_BEGIN_TRANSMIT_LEADERBOARD);

	for (int i = 2; i <= sizeArray; i++) {
		char string[50] = "hey delilah";
		int randNum = 5493;
		if (receive_int(sockfd) != BEGIN_TRANSMIT_LEADERBOARD) {
			printf("Error receiving begin transmit leaderboard\n");
			return CODE_ERROR;
		}

		send_int(sockfd, BEGIN_TRANSMIT_LEADERBOARD_ENTRY);

		// Username
		send_char(sockfd, string);
		if ((receive_char(sockfd, string)) != ACKNOWLEDGE_RECEIVED_USERNAME) {
			printf("Error transmitting leaderboard username entry %d\n", i);
			return CODE_ERROR;
		}

		send_int(sockfd, randNum);
		if (receive_int(sockfd) != CODE_SUCCESS) {
			printf("Error transmitiing leaderboard seconds entry %d\n", i);
			return CODE_ERROR;
		}

		send_int(sockfd, randNum);
		if (receive_int(sockfd) != CODE_SUCCESS) {
			printf("Error transmitiing leaderboard games played entry %d\n", i);
			return CODE_ERROR;
		}

		send_int(sockfd, randNum);
		if (receive_int(sockfd) != CODE_SUCCESS) {
			printf("Error transmitiing leaderboard games won entry %d\n", i);
			return CODE_ERROR;
		}
		if (i == sizeArray) {
			send_int(sockfd, END_TRANSMIT_LEADERBOARD);
			if (receive_int(sockfd) != ACKNOWLEDGE_END_TRANSMIT_LEADERBOARD) {
				printf("Error transmitting end of leaderboard %d\n", i);
				return CODE_ERROR;
			}
			return CODE_SUCCESS;
		}
		send_int(sockfd, ACKNOWLEDGE_END_LEADERBOARD_ENTRY);

	}
}
/*
 * @brief receive username and password from client to authenticate connection
 * @arg sockfd the socket id to receive credentials via
 * @return -1 on failure, 1 on success
 */
int authenticate(int sockfd, char usernames[][MAXSTRINGSIZE], char passwords[][MAXSTRINGSIZE], int sizeArray, User user) {
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
			return CODE_SUCCESS;
		}
	}
     send_int(sockfd, AUTHENTICATE_FAILURE);
     return CODE_ERROR;

}

/*
int main(int argc, char *argv[])
{
	// Get port number for server to listen on
	if (argc != 2) {
		fprintf(stderr,"usage: client port_number\n");
		exit(1);
	}

     int sockfd = start_listen_server(argv[1]);
     printf("[SERVER] Listen server started on port %s.\n", argv[1]);

     int newfd = connect_to_client(sockfd);
     printf("[SERVER] Established connection to client.\n");

     bool GAME_END = false;
     int instruction, instruction_old;
     char result[MAXDATASIZE];
     int success = -1;

     while (!GAME_END) {
          instruction = receive_int(newfd);
          if (instruction != instruction_old) {
               if (instruction == 999) {
                    GAME_END = true;
               }
               else if (instruction == BEGIN_AUTHENTICATE) {
                    success = authenticate(newfd);
                    if (success == CODE_SUCCESS) {
                         printf("User successfully authenticated\n");
                    }
                    else if (success == CODE_ERROR) {
                         printf("User failed to authenticate\n");
                    }
                    else {
                         printf("Something went horribly wrong trying to auth the user\n");
                    }
                    continue;
               }
               else if (instruction == BEGIN_TRANSMIT_STRING) {
                    receive_char(newfd, result);
                    printf("Received: %s\n", result);

                    continue;

               }
               else {
                    printf("instruction: %d\n", instruction);
                    instruction_old = instruction;
                    continue;
               }
          }
          else {
               continue;
          }
     }


     close(sockfd);
     close(newfd);
     printf("[SERVER] Closed both sockets and exiting...\n");

     return CODE_SUCCESS;
}

*/

#endif
