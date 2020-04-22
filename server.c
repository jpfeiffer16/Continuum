#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#define PORT 8080
#define SA struct sockaddr

int main()
{
  int sockfd, connfd, len;
  struct sockaddr_in servaddr = {};

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1) {
    printf("socket creation failed...\n");
    exit(0);
  }

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
    printf("Failed to bind socket\n");
    exit(0);
  }

  // Now server is ready to listen and verification
  if ((listen(sockfd, 8)) != 0) {
    printf("Listen failed\n");
    exit(0);
  }

  printf("Server listening...\n");
}
