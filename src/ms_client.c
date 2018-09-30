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
#include "inc/networking.h"

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
 * Communication to server. General functionality to be utilised further soon.
 */
void Send_Array_Data(int socket_id, int *myArray) {
	int i=0;
	uint16_t statistics;
	for (i = 0; i < ARRAY_SIZE; i++) {
		statistics = htons(myArray[i]);
		send(socket_id, &statistics, sizeof(uint16_t), 0);
	}
}

int connect(int argc, char *argv[]) {
	int sockfd, numbytes, i=0;  
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	if (argc != 3) {
		fprintf(stderr,"usage: client_hostname port_number\n");
		return 1;
	}

	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		herror("gethostbyname");
		return 1;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return 1;
	}


	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(atoi(argv[2]));    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
		perror("connect");
		return 1;
	}

	/* Create an array of squares of first 30 whole numbers */
	int simpleArray[ARRAY_SIZE] = {0};
	for (i = 0; i < ARRAY_SIZE; i++) {
		simpleArray[i] = i * i;
	}

	Send_Array_Data(sockfd, simpleArray);

	/* Receive message back from server */
	if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		return 1;
	}

	buf[numbytes] = '\0';

	buf[numbytes] = '\0';

	printf("Received: %s",buf);

	close(sockfd);

	return 0;
}



/*
* Main.
*/
int main(int argc, char *argv[]) {
     int result = connect();
     exit(result);
}
