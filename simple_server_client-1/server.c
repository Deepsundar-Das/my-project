#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<pthread.h>
// define Macros
#define SERVER_PORT 8080

void *recv_mess(void *server_fd)
{
  int recv_server_fd = *(int *)server_fd;
  while (1)
  {
    int recv_data_descriptor;
    char BUFFER[1024];
    recv_data_descriptor = recv(recv_server_fd, BUFFER, 1023, 0);
    //* 6. read data from server
    if (recv_data_descriptor < 0)
    {
      printf("data retrival is failed\n");
      close(recv_server_fd);
      exit(EXIT_FAILURE);
    }
    else if (recv_data_descriptor == 0)
    {
      printf("connection closed by server\n");
      close(recv_server_fd);
      exit(EXIT_FAILURE);
    }
    BUFFER[recv_data_descriptor] = '\0';

    printf("server -> %s\n", BUFFER);
    memset(BUFFER, 0, strlen(BUFFER));
  }
  return NULL;
}

int main() {
  int child_socket_fd;
  int server_fd;
  char BUFFER[1024] = {0};
  int recv_data_descriptor;
  // int socket(int domain, int type, int protocol)
  // domain -> protocol family(ipv4, ipv6), ipv4 -> AF_INET
  // type -> transport layer protocol(eg: tcp, udp, etc.)
  // protocol -> some fucking fancy algorithm, 0 -> use default available
  // protocol

  struct sockaddr_in socket_address;
  int socket_address_length = sizeof(socket_address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Failed to create tcp socket...\n");
    exit(EXIT_FAILURE);
  } else {
    printf("TCP socket created with file descriptor -> %d\n", server_fd);
  }
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = INADDR_ANY;
  socket_address.sin_port =
      htons(SERVER_PORT); // Convert port to network byte order

  // 3. Bind socket to the socket_address and port
  if (bind(server_fd, (struct sockaddr *)&socket_address,
           sizeof(socket_address)) < 0) {
    perror("Bind failed\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // 4. Start listening for incoming connections (Queue up to 3 connections)
  if (listen(server_fd, 3) < 0) {
    perror("Listen failed\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  printf("Server is listening on port %d...\n", SERVER_PORT);

  if ((child_socket_fd =
           accept(server_fd, (struct sockaddr *)&socket_address,
                  (socklen_t *)&socket_address_length)) < 0) {
    printf("accept failed\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("client connected\n");
  pthread_t recv_thread;
  pthread_create(&recv_thread, NULL, recv_mess, (void *)&child_socket_fd);
  while (1) {
    // recv_data_descriptor = recv(child_socket_fd, BUFFER, 1023, 0);
    // if (recv_data_descriptor < 0) {
    //   printf("data retrival is failed, file descriptor -> %d\n",
    //          recv_data_descriptor);
    //   close(child_socket_fd);
    //   exit(EXIT_FAILURE);
    // } else if (recv_data_descriptor == 0) {
    //   printf("connection is closed by client\n");
    //   close(child_socket_fd);
    //   exit(EXIT_FAILURE);
    // }
    // printf("client -> %s\n", BUFFER);
    // memset(BUFFER, 0, strlen(BUFFER));

    // char *message = "hello from server side, are you ready for data "
    //                 "transfer(innocent face)";
    char message[1024];
    char *msg_ptr = message;
    while((*msg_ptr = (char)getchar()) != '\n'){
        msg_ptr++;
    }
    send(child_socket_fd, message, strlen(message), 0);
    memset(message, 0, strlen(message));

  }
  // close(server_fd);
  close(child_socket_fd);
  // exit(EXIT_SUCCESS);
  // return 0;
}
