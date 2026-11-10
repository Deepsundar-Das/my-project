#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

// define Macros
#define SERVER_PORT 8080

int main(){
  int client_socket;
  int server_socket;
  // int socket(int domain, int type, int protocol)
  // domain -> protocol family(ipv4, ipv6), ipv4 -> AF_INET
  // type -> transport layer protocol(eg: tcp, udp, etc.)
  // protocol -> some fucking fancy algorithm, 0 -> use default available protocol

  struct sockaddr_in socket_address;
  int socket_address_length = sizeof(socket_address);

  if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("Failed to create tcp socket...");
  }
  else{
    printf("TCP socket created with file descriptor -> %d",server_socket);
  }
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = INADDR_ANY; 
  socket_address.sin_port = htons(SERVER_PORT);       // Convert port to network byte order

  // 3. Bind socket to the socket_address and port
  if (bind(server_socket, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0) {
      perror("Bind failed");
      close(server_socket);
      exit(EXIT_FAILURE);
  }

  // 4. Start listening for incoming connections (Queue up to 3 connections)
  if (listen(server_socket, 3) < 0) {
      perror("Listen failed");
      close(server_socket);
      exit(EXIT_FAILURE);
  }
  printf("Server is listening on port %d...\n", SERVER_PORT);

  if (client_socket = accept(server_socket, (struct sockaddr*)&socket_address, (socklen_t*)&socket_address_length) < 0){
    printf("accept failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("client connected");

  char *message = "hello from server side, are you ready for data transfer";
  send(client_socket, message, strlen(message), 0);
  close(server_socket);
  close(client_socket);
  return 0;
}

