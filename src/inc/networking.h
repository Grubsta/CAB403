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

#define CODE_SUCCESS 0
#define CODE_ERROR -1


void send_int(int sockfd, int value) {
     uint16_t statistics;

     statistics = htons(value);
     send(sockfd, &statistics, sizeof(uint16_t), 0);
}

int receive_int(int sockfd) {
    uint16_t statistics;
    int results;

    while (1) {
          if ((recv(sockfd, &statistics, sizeof(uint16_t), 0)) == -1) {
               printf("receive failed\n");
          }
          else {
               break;
          }
     }

     results = ntohs(statistics);

     return results;
}

int send_char(int sockfd, char * value) {
     // Notify server that char is about to be sent
     send_int(sockfd, 500);

     // Wait for server to acknowledge the outgoing char
     if (receive_int(sockfd) != 501) {
          printf("Error receiving acknowledgement from socket\n");
          return CODE_ERROR;
     }

     // Send the char to the server
     send(sockfd, value, MAXDATASIZE, 0);

     // Wait for the server to acknowledge the char has been received
     if (receive_int(sockfd) != 502) {
          printf("Error receiving received acknowledgement from socket\n");
          return CODE_ERROR;
     }

     // Notify the server that the char transmission is over
     send_int(sockfd, 503);

     if (receive_int(sockfd) != 504) {
          printf("Error receiving closing acknowledgement from socket\n");
          return CODE_ERROR;
     }

     return CODE_SUCCESS;
}

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
