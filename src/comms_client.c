#include "inc/networking.h"

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

int main(int argc, char*argv[]) {
	if (argc != 3) {
		fprintf(stderr,"usage: client_hostname port_number\n");
		exit(1);
	}

     int sockfd = connect_to_server(argv[1], argv[2]);
     printf("[CLIENT] Connection established with server.\n");

     char test[512] = "This is a test string";
     printf("Sending: %s\n", test);
     send_char(sockfd, test);

     send_int(sockfd, 999);

     close(sockfd);
     return CODE_SUCCESS;
}
