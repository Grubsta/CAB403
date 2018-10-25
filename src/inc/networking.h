#include <arpa/inet.h>
#include <errno.h>
#include <errno.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdbool.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>

// Shared variables (client, server)
#define MAXDATASIZE 100
#define BACKLOG 10

// Error codes
#define CODE_SUCCESS 0
#define CODE_ERROR -1

// PROTOCOL MAP

// Authentication
#define BEGIN_AUTHENTICATE 400
#define ACKNOWLEDGE_BEGIN_AUTHENTICATE 401
#define ACKNOWLEDGE_RECEIVED_USERNAME 402
#define ACKNOWLEDGE_RECEIVED_PASSWORD 403
#define AUTHENTICATE_SUCCESS 404
#define AUTHENTICATE_FAILURE 405
#define END_AUTHENTICATE 405
#define ACKNOWLEDGE_END_AUTHENTICATE 406

// String tranmission
#define BEGIN_TRANSMIT_STRING 500
#define ACKNOWLEDGE_BEGIN_TRANSMIT_STRING 501
#define ACKNOWLEDGE_RECEIVED_STRING 502
#define END_TRANSMIT_STRING 503
#define ACKNOWLEDGE_END_TRANSMIT_STRING 504


/*
 * @brief send an int via the socket
 * @arg sockfd the socket to communicate over
 * @arg value the int to send via the socket
*/
void send_int(int sockfd, int value) {
     uint16_t statistics;

     statistics = htons(value);
     send(sockfd, &statistics, sizeof(uint16_t), 0);
}

/*
 * @brief receive an int via the socket
 * @arg sockfd the socket id to communicate over
 * @return the value received
*/
int receive_int(int sockfd) {
    uint16_t statistics;
    int results;

    while (1) {
          if ((recv(sockfd, &statistics, sizeof(uint16_t), 0)) == CODE_ERROR) {
               printf("receive failed\n");
          }
          else {
               break;
          }
     }

     results = ntohs(statistics);

     return results;
}

/*
 * @brief send a string via the socket
 * @arg sockfd the socket id to communicate over
 * @arg value the string to send
 * @return -1 on fail, 0 on success
*/
int send_char(int sockfd, char * value) {
     // Notify server that char is about to be sent
     send_int(sockfd, BEGIN_TRANSMIT_STRING);

     // Wait for server to acknowledge the outgoing char
     if (receive_int(sockfd) != ACKNOWLEDGE_BEGIN_TRANSMIT_STRING) {
          printf("Error receiving acknowledgement from socket\n");
          return CODE_ERROR;
     }

     // Send the char to the socket
     send(sockfd, value, MAXDATASIZE, 0);

     // Wait for the socket to acknowledge the char has been received
     if (receive_int(sockfd) != ACKNOWLEDGE_RECEIVED_STRING) {
          printf("Error receiving received acknowledgement from socket\n");
          return CODE_ERROR;
     }

     // Notify the socket that the char transmission is over
     send_int(sockfd, END_TRANSMIT_STRING);

     if (receive_int(sockfd) != ACKNOWLEDGE_END_TRANSMIT_STRING) {
          printf("Error receiving closing acknowledgement from socket\n");
          return CODE_ERROR;
     }

     return CODE_SUCCESS;
}

/*
 * @brief receive a string via the socket
 * @arg sockfd the socket to communicate over
 * @arg destination the destination string to write to
 * @return -1 on error, 0 on success
*/
int receive_char(int sockfd, char * destination) {
     // Send acknowledgement to client
     send_int(sockfd, 501);

     // Receive the incoming char
     int numbytes = recv(sockfd, destination, MAXDATASIZE, 0);
     destination[numbytes] = '\0';

     // Notify the client that the char has been received
     send_int(sockfd, 502);

     // Wait for the client to notify the end of the tranmission
     if (receive_int(sockfd) != 503) {
          printf("Error receiving closing notice from socket\n");
          return CODE_ERROR;
     }

     send_int(sockfd, 504);

     return CODE_SUCCESS;
}
