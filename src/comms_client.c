#ifndef COMMS_CLIENT_C
#define COMMS_CLIENT_C

#include "inc/networking.h"
#include "inc/includes.h"

/*
 * @brief connect to a currently running server
 * @arg client_hostname the hostname of the server to connect to
 * @arg port_number the port number to communicate to the server through
 * @return socket id of server connection
*/
int connect_to_server(char * client_hostname, char * port_number) {
	int sockfd;                        /* the socket ID to communicate through */ 
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

void disconnect_from_server(int sockfd) {
     send_int(sockfd, END_CONNECTION);
     close(sockfd);
}

int cmd_place_flag(int sockfd, char coordinates[2]) {
     int y = -1;
     int x = -1;
     int i;

     for (i = 0; i < NUM_TILES_Y; i++) {
          printf("y: %i\n", i);
          if (coordinates[0] == y_Axis[i]) {
               y = i;
               break;
          }
     }

     for (i = 0; i < NUM_TILES_X; i++) {
          printf("x: %i\n", i);
          if (coordinates[1] == x_Axis[i]) {
               x = i;
               break;
          }
     }

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

     if (receive_int(sockfd) != END_COMMAND) {
          printf("Error receiving end command notice from sevrer\n");
          return CODE_ERROR;
     }

     send_int(sockfd, ACKNOWLEDGE_END_COMMAND);

     return CODE_SUCCESS;
}

/*
 * @brief send username and password to server for authentication
 * @arg sockfd the socket id to send credentials via
 * @return -1 on failure, 0 on success
 */
int authenticate(int sockfd, char * username, char * password) {
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
