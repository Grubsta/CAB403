#ifndef NETWORKING_H
#define NETWORKING_H

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

// Client to server commands
#define BEGIN_COMMAND 300
#define ACKNOWLEDGE_BEGIN_COMMAND 301
#define END_COMMAND 302
#define ACKNOWLEDGE_END_COMMAND 303
#define COMMAND_PLACE_FLAG 310
#define COMMAND_REVEAL_TILE 311
#define COMMAND_PLACE_FLAG_FAIL 312
#define COMMAND_PLACE_FLAG_SUCCESS 313

// Authentication
#define BEGIN_AUTHENTICATE 400
#define ACKNOWLEDGE_BEGIN_AUTHENTICATE 401
#define ACKNOWLEDGE_RECEIVED_USERNAME 402
#define ACKNOWLEDGE_RECEIVED_PASSWORD 403
#define AUTHENTICATE_SUCCESS 404
#define AUTHENTICATE_FAILURE 405

// String tranmission
#define BEGIN_TRANSMIT_STRING 500
#define ACKNOWLEDGE_BEGIN_TRANSMIT_STRING 501
#define ACKNOWLEDGE_RECEIVED_STRING 502
#define END_TRANSMIT_STRING 503
#define ACKNOWLEDGE_END_TRANSMIT_STRING 504

// Leader Board transmission
#define BEGIN_TRANSMIT_LEADERBOARD 600
#define ACKNOWLEDGE_BEGIN_TRANSMIT_LEADERBOARD 601
#define ACKNOWLEDGE_RECIEVED_LEADERBOARD 602
#define BEGIN_TRANSMIT_LEADERBOARD_ENTRY 603
#define ACKNOWLEDGE_BEGIN_LEADERBOARD_ENTRY 604
#define END_TRANSMIT_LEADERBOARD_ENTRY 605
#define ACKNOWLEDGE_END_LEADERBOARD_ENTRY 606
#define END_TRANSMIT_LEADERBOARD 607
#define ACKNOWLEDGE_END_TRANSMIT_LEADERBOARD 608

// Grid reveal
#define BEGIN_GRID_REVEAL 700
#define ACKNOWLEDGE_BEGIN_GRID_REVEAL 701
#define END_GRID_REVEAL 702
#define ACKNOWLEDGE_END_GRID_REVEAL 703

// Game state
#define BEGIN_GAME 800
#define ACKNOWLEDGE_BEGIN_GAME 801
#define END_GAME 802
#define ACKNOWLEDGE_END_GAME 803
#define GAME_OVER 804
#define ACKNOWLEDGE_GAME_OVER 805
#define GAME_WON 806
#define ACKNOWLEDGE_GAME_WON 807

// Connection
#define END_CONNECTION 999

/*
 * @brief send an int via the socket
 * @arg sockfd the socket to communicate over
 * @arg value the int to send via the socket
*/
void send_int(int sockfd, int value) {
     uint16_t statistics;

     statistics = htons(value);
     printf("Sent: %d\n", value);
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

     printf("Received: %d\n", results);

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
     send_int(sockfd, ACKNOWLEDGE_BEGIN_TRANSMIT_STRING);

     // Receive the incoming char
     int numbytes = recv(sockfd, destination, MAXDATASIZE, 0);
     destination[numbytes] = '\0';

     // Notify the client that the char has been received
     send_int(sockfd, ACKNOWLEDGE_RECEIVED_STRING);

     // Wait for the client to notify the end of the tranmission
     if (receive_int(sockfd) != END_TRANSMIT_STRING) {
          printf("Error receiving closing notice from socket\n");
          return CODE_ERROR;
     }

     send_int(sockfd, ACKNOWLEDGE_END_TRANSMIT_STRING);

     return CODE_SUCCESS;
}

#endif
