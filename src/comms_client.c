#ifndef COMMS_CLIENT_C
#define COMMS_CLIENT_C

#include "inc/networking.h"
#include "inc/includes.h"

// The socket ID to communicate through
int sockfd = -1;

// The amount of flags the client has remaining
int flags_remaining = FLAGS;

/*
 * @brief connect to a currently running server
 * @arg client_hostname the hostname of the server to connect to
 * @arg port_number the port number to communicate to the server through
 * @return socket id of server connection
*/
int connect_to_server(char * client_hostname, char * port_number) {
	struct hostent *he;                /* host information */
	struct sockaddr_in server_addr;    /* connector's address information */

     // Getting the host information via the supplied hostname
	if ((he = gethostbyname(client_hostname)) == NULL) {
          herror("gethostbyname");
          exit(CODE_ERROR);
     }

     // Obtaining a socket connection
     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == CODE_ERROR) {
          herror("socket");
          exit(CODE_ERROR);
     }

	server_addr.sin_family = AF_INET;                           /* host byte order */
	server_addr.sin_port = htons(atoi(port_number));            /* short, network byte order */
	server_addr.sin_addr = *((struct in_addr *)he->h_addr);     /* the server host address */
	bzero(&(server_addr.sin_zero), 8);                          /* zero the rest of the struct */

     // Attempting to connect to the server using the details retrieved above
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == CODE_ERROR) {
          perror("connect");
          exit(1);
     }

     // Returning the socket ID on successful connection
	return sockfd;
}

/*
 * @brief disconnect from the specified socket
 */
void disconnect_from_server() {
     send_int(sockfd, END_CONNECTION);
     close(sockfd);
}

/*
 * @brief method to retrieve the entire grid. relies on reveal_grid function in comms_server
 * @return -1 on fail, 0 on success
 */
int reveal_grid() {
     if (receive_int(sockfd) != BEGIN_GRID_REVEAL) {
          printf("Error receiving grid reveal notice from server");
          return CODE_ERROR;
     }
     send_int(sockfd, ACKNOWLEDGE_BEGIN_GRID_REVEAL);

     int instruction, y, x, value;
     char tile[0];

     while (1) {
          y = receive_int(sockfd);
          if (y == END_GRID_REVEAL) {
               break;
          }
          send_int(sockfd, y);

          x = receive_int(sockfd);
          if (x == END_GRID_REVEAL) {
               break;
          }
          send_int(sockfd, x);

          value = receive_int(sockfd);
          if (value == END_GRID_REVEAL) {
               break;
          }
          send_int(sockfd, value);

          if (value > 8) {
               tile[0] = '*';
          }
          else {
               tile[0] = value + '0';
          }

          grid[y][x] = *tile;
     }

     send_int(sockfd, ACKNOWLEDGE_END_GRID_REVEAL);
     return CODE_SUCCESS;
}

/*
 * @brief command to send coordinate selection to server for processing
 * @arg coordinates the coordinates string (2 characters) to send to the server
 * return -1 on fail, 0 on success
 */
int cmd_reveal_tile(int y, int x) {
     send_int(sockfd, BEGIN_COMMAND);

     if (receive_int(sockfd) != ACKNOWLEDGE_BEGIN_COMMAND) {
          printf("Error receiving command acknowledgement from server\n");
          return CODE_ERROR;
     }


     send_int(sockfd, COMMAND_REVEAL_TILE);

     if (receive_int(sockfd) != COMMAND_REVEAL_TILE) {
          printf("Error receiving command type acknowledgement from server\n");
          return CODE_ERROR;
     }

     send_int(sockfd, y);

     send_int(sockfd, x);

     int result = receive_int(sockfd);
     char tile[0];
     tile[0] = result + '0';
     grid[y][x] = *tile;


     // Check if game is over, won, or continuing
     int status = receive_int(sockfd);
     switch (status) {
          case GAME_OVER:
               send_int(sockfd, ACKNOWLEDGE_GAME_OVER);
               return GAME_OVER;
               break;
          case GAME_WON:
               send_int(sockfd, ACKNOWLEDGE_GAME_WON);
               return GAME_WON;
               break;
          case END_COMMAND:
               send_int(sockfd, ACKNOWLEDGE_END_COMMAND);
               break;
          default:
               printf("Error receiving end command notice from server\n");
               return CODE_ERROR;
               break;
     }

     return CODE_SUCCESS;
}

/*
 * @brief command to send coordinate selection to server for processing
 * @arg coordinates the coordinates string (2 characters) to send to the server
 * return -1 on fail, 0 on success
 */
int cmd_place_flag(int y, int x) {
     send_int(sockfd, BEGIN_COMMAND);

     if (receive_int(sockfd) != ACKNOWLEDGE_BEGIN_COMMAND) {
          printf("Error receiving command acknowledgement from server\n");
          return CODE_ERROR;
     }


     send_int(sockfd, COMMAND_PLACE_FLAG);

     if (receive_int(sockfd) != COMMAND_PLACE_FLAG) {
          printf("Error receiving command type acknowledgement from server\n");
          return CODE_ERROR;
     }

     send_int(sockfd, y);

     send_int(sockfd, x);

     int flagstate = receive_int(sockfd);
     switch (flagstate) {
          case COMMAND_PLACE_FLAG_SUCCESS:
               printf("Made it here\n");
               grid[y][x] = '+';
               flags_remaining--;
               break;
          case COMMAND_PLACE_FLAG_FAIL:
               printf("Cannot place flag\n");
               break;
          default:
               printf("Error receiving flag confirmation from server\n");
               return CODE_ERROR;
     }

     //Check if game is over, won, or continuing
     int status = receive_int(sockfd);
     switch (status) {
          case GAME_OVER:
               send_int(sockfd, ACKNOWLEDGE_GAME_OVER);
               return GAME_OVER;
               break;
          case GAME_WON:
               send_int(sockfd, ACKNOWLEDGE_GAME_WON);
               return GAME_WON;
               break;
          case END_COMMAND:
               send_int(sockfd, ACKNOWLEDGE_END_COMMAND);
               break;
          default:
               printf("Error receiving end command notice from server\n");
               return CODE_ERROR;
               break;
     }


     return CODE_SUCCESS;
}

/*
 * @brief send username and password to server for authentication
 * @return -1 on failure, 0 on success
 */
int authenticate(char * username, char * password) {
     send_int(sockfd, BEGIN_AUTHENTICATE);

     if (receive_int(sockfd) != ACKNOWLEDGE_BEGIN_AUTHENTICATE) {
          printf("Error receiving authentication acknowledgement from server\n");
          return CODE_ERROR;
     }

     if (send_char(sockfd, username) != CODE_SUCCESS) {
          printf("Error transmitting username to server\n");
          return CODE_ERROR;
     }

     if (receive_int(sockfd) != ACKNOWLEDGE_RECEIVED_USERNAME) {
          printf("Error receiving acknowledgement of username from server\n");
          return CODE_ERROR;
     }

     if (send_char(sockfd, password) != CODE_SUCCESS) {
          printf("Error transmitting password to server\n");
          return CODE_ERROR;
     }

     if (receive_int(sockfd) != ACKNOWLEDGE_RECEIVED_PASSWORD) {
          printf("Error receiving acknowledgement of password from server\n");
          return CODE_ERROR;
     }

     int result = receive_int(sockfd);
     if (result == AUTHENTICATE_SUCCESS) {
          return CODE_SUCCESS;
     }

     return CODE_ERROR;
}

int generateLeaderboard(char * username, int results[3]) {
     send_int(sockfd, BEGIN_TRANSMIT_LEADERBOARD);

	if (receive_int(sockfd) != ACKNOWLEDGE_BEGIN_TRANSMIT_LEADERBOARD) {
          printf("Error receiving ackowledgement for request of leaderboard\n");
          return CODE_ERROR;
     }

     if (receive_int(sockfd) != BEGIN_TRANSMIT_LEADERBOARD_ENTRY) {
          printf("Error receiving begin transmit notice for leaderboard entry\n");
          return CODE_ERROR;
     }

     send_int(sockfd, ACKNOWLEDGE_BEGIN_LEADERBOARD_ENTRY);

     if (receive_int(sockfd) != BEGIN_TRANSMIT_STRING) {
          printf("Error receiving beginning of string transmission for leaderboard username\n");
          return CODE_ERROR;
     }

     if (receive_char(sockfd, username) != CODE_SUCCESS) {
          printf("Error receiving username of leaderboard entry\n");
          return CODE_ERROR;
     }

     int seconds = receive_int(sockfd);
     int numGames = receive_int(sockfd);
     int gamesWon = receive_int(sockfd);

     if (receive_int(sockfd) != END_TRANSMIT_LEADERBOARD_ENTRY) {
          printf("Error receiving end of leaderboard entry notice\n");
          return CODE_ERROR;
     }

     send_int(sockfd, ACKNOWLEDGE_END_LEADERBOARD_ENTRY);

     if (receive_int(sockfd) != END_TRANSMIT_LEADERBOARD) {
          printf("Error receiving end of leaderboard notice\n");
          return CODE_ERROR;
     }

     send_int(sockfd, ACKNOWLEDGE_END_TRANSMIT_LEADERBOARD);

     results[0] = seconds;
     results[1] = numGames;
     results[2] = gamesWon;

     return CODE_SUCCESS;
}


/*
int main(int argc, char*argv[]) {
	if (argc != 3) {
		fprintf(stderr,"usage: client_hostname port_number\n");
		exit(1);
	}

     int sockfd = connect_to_server(argv[1], argv[2]);
     printf("[CLIENT] Connection established with server.\n");


     char username[MAXDATASIZE] = "jack";
     char password[MAXDATASIZE] = "password";

     int success = authenticate(sockfd, username, password);

     if (success == CODE_SUCCESS) {
          printf("Successfully authenticated with server\n");
     }
     else if (success == CODE_ERROR) {
          printf("Failed to authenticate with server\n");
     }
     else {
          printf("SOmething has gone terribly wrong\n");
     }

     //char test[MAXDATASIZE] = "This is a test string";
     //printf("Sending: %s\n", test);
     //send_char(sockfd, test);

     send_int(sockfd, 999);

     close(sockfd);
     return CODE_SUCCESS;
}
*/

#endif
