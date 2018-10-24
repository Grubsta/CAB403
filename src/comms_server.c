#include "inc/networking.h"

int start_listen_server(char * port_number) {
	int sockfd;  /* listen on sock_fd, new connection on newfd */
	struct sockaddr_in server_addr;    /* my address information */
	struct sockaddr_in client_addr; /* connector's address information */


	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
          perror("socket");
          exit(1);
     }

	/* generate the end point */
	server_addr.sin_family = AF_INET;         /* host byte order */
	server_addr.sin_port = htons(atoi(port_number));     /* short, network byte order */
	server_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */

	/* bind the socket to the end point */
     if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
          perror("bind");
          exit(1);
     }

	/* start listnening */
     if (listen(sockfd, 10) == -1) {
          perror("listen");
          exit(1);
     }

     return sockfd;
}

int connect_to_client(int sockfd) {
     int newfd;
     struct sockaddr_in client_addr;
	socklen_t sin_size;
     bool done = false;

     done = false;
     while(!done) {
          if ((newfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
               continue;
          }

          done = true;
     }
     return newfd;
}

int main(int argc, char *argv[])
{
	/* Get port number for server to listen on */
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

     while (!GAME_END) {
          instruction = receive_int(newfd);
          if (instruction != instruction_old) {
               if (instruction == 999) {
                    GAME_END = true;
               }
               else if (instruction == 500) {
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

     return 0;
}
