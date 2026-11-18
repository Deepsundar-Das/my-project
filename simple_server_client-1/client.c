#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

void *recv_mess(void *sock_fd)
{
  int recv_sock_fd = *(int *)sock_fd;
  while (1)
  {
    int recv_data_descriptor;
    char BUFFER[1024];
    recv_data_descriptor = recv(recv_sock_fd, BUFFER, 1023, 0);
    //* 6. read data from server
    if (recv_data_descriptor < 0)
    {
      printf("data retrival is failed\n");
      close(recv_sock_fd);
      exit(EXIT_FAILURE);
    }
    else if (recv_data_descriptor == 0)
    {
      printf("connection closed by server\n");
      close(recv_sock_fd);
      exit(EXIT_FAILURE);
    }
    BUFFER[recv_data_descriptor] = '\0';

    printf("server -> %s\n", BUFFER);
    memset(BUFFER, 0, strlen(BUFFER));
  }
  return NULL;
}

// void close_conn()
// TODO: do not forget to close the sockets
int main()
{
  //! fd -> file descriptor
  //! after calling socket() -> creates a socket and update the current
  //! process's process table that is fd 3(open file) is pointing to the socket
  //! in case of forking all the things with the process table is copied as it
  //! is
  int sock_fd;
  int recv_data_descriptor;
  char *cl_ip = "127.0.0.1";
  char *serv_ip = "127.0.0.1";
  struct sockaddr_in cl_addr;
  struct sockaddr_in serv_addr;
  // char *message_from_cl =
  //     "Hi, I am client. Are you ready for a handshake(dark smile)";
  char BUFFER[1024] = {0};

// Macros
// AF -> Address Family
#define IPV4 AF_INET
// #define CL_PORT 8000
#define SERV_PORT 8080
#define CL_ADDR_SIZE sizeof(cl_addr)

  //* 1. create a unbound socket
  if ((sock_fd = socket(IPV4, SOCK_STREAM, 0)) < 0)
  {
    printf("socket cannot be created.... ");
    exit(EXIT_FAILURE);
  }
  printf("socket is created with file descriptor: %d\n", sock_fd);

  //* 2. create address
  cl_addr.sin_family = IPV4;
  cl_addr.sin_port = htons(0);
  if (inet_pton(IPV4, cl_ip, &cl_addr.sin_addr) < 0)
  {
    printf("invalid ip or not supported\n");
    close(sock_fd);
    exit(EXIT_FAILURE);
  }
  printf("client address creation is successful....\n");
  printf("client port number - %d\n", cl_addr.sin_port);

  //* 3. bind socket
  if (bind(sock_fd, (struct sockaddr *)&cl_addr, CL_ADDR_SIZE) < 0)
  {
    printf("socket binding with address cannot be done\n");
    close(sock_fd);
    exit(EXIT_FAILURE);
  }
  printf("socket binding with address is successful....\n");

  serv_addr.sin_port = htons(SERV_PORT);
  serv_addr.sin_family = IPV4;
  if ((inet_pton(IPV4, serv_ip, &serv_addr.sin_addr)) < 0)
  {
    printf("server ip assigning is failed\n");
    close(sock_fd);
    exit(EXIT_FAILURE);
  }

  //* 4. connect(create a session)
  if (connect(sock_fd, (struct sockaddr *)&serv_addr, CL_ADDR_SIZE) < 0)
  {
    printf("session establishment failed\n");
    close(sock_fd);
    exit(EXIT_FAILURE);
  }

  printf("session established successfully\n");

  pthread_t recv_thread;
  pthread_create(&recv_thread, NULL, recv_mess, (void *)&sock_fd);

  time_t exit_time;
  do
  {
    char message[1024];
    char *msg_ptr = &message[0];

    // scanf("%s", message);
    while ((*msg_ptr = (char)getchar()) != '\n')
    {
      msg_ptr++;
      // printf("the charcter is %d and address is %zu", *--msg_ptr, &msg_ptr);
    };
    *msg_ptr = '\0';

    //* 5. send data
    if (send(sock_fd, message, strlen(message), 0) < 0)
    {
      printf("failed to send message\n");
      close(sock_fd);
      exit(EXIT_FAILURE);
    }
    memset(message, 0, strlen(message));

    time(&exit_time);

    // recv_data_descriptor = recv(sock_fd, BUFFER, 1023, 0);
    // //* 6. read data from server
    // if (recv_data_descriptor < 0) {
    //   printf("data retrival is failed\n");
    //   close(sock_fd);
    //   exit(EXIT_FAILURE);
    // } else if (recv_data_descriptor == 0) {
    //   printf("connection closed by server\n");
    //   close(sock_fd);
    //   exit(EXIT_FAILURE);
    // }

    // printf("server -> %s\n", BUFFER);
    // memset(BUFFER, 0, strlen(BUFFER));
    // time_t exit_time = time(NULL);
    //! or you can do this
    // time_t exit_time;

  } while (1);
  close(sock_fd);
  exit(EXIT_SUCCESS);

  return 0;
}
