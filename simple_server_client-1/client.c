#include <netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
// void close_conn()
//TODO: do not forget to close the sockets
int main(){
    //! fd -> file descriptor
    //! after calling socket() -> creates a socket and update the current process's process table that is fd 3(open file) is pointing to the socket
    //! in case of forking all the things with the process table is copied as it is
    int sock_fd;
    int recv_data_descriptor;
    char *cl_ip = "127.0.0.1";
    char *serv_ip = "127.0.0.1";
    struct sockaddr_in cl_addr;
    struct sockaddr_in serv_addr;
    char *message_from_cl = "Hi, I am client. Are you ready for a handshake(dark smile)";
    char BUFFER[1024] = {0};


    // Macros
    // AF -> Address Family
    #define IPV4 AF_INET
    // #define CL_PORT 8000
    #define SERV_PORT 8080
    #define CL_ADDR_SIZE sizeof(cl_addr)

    //* 1. create a unbound socket
    if ((sock_fd = socket(IPV4, SOCK_STREAM, 0)) < 0){
        printf("socket cannot be created.... ");
        exit(EXIT_FAILURE);
    }
    printf("socket is created with file descriptor: %d\n", sock_fd);

    //* 2. create address
    cl_addr.sin_family = IPV4;
    cl_addr.sin_port = htons(0);
    if (inet_pton(IPV4, cl_ip, &cl_addr.sin_addr) < 0){
        printf("invalid ip or not supported\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    printf("client address creation is successful....\n");
    printf("client port number - %d\n", cl_addr.sin_port);

    //* 3. bind socket
    if (bind(sock_fd, (struct sockaddr*)&cl_addr, CL_ADDR_SIZE) < 0){
        printf("socket binding with address cannot be done\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    printf("socket binding with address is successful....\n");

    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_family = IPV4;
    if ((inet_pton(IPV4, serv_ip, &serv_addr.sin_addr)) < 0){
        printf("server ip assigning is failed\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }


    //* 4. connect(create a session)
    if (connect(sock_fd, (struct sockaddr*)&serv_addr, CL_ADDR_SIZE) < 0){
        printf("session establishment failed\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }


    //* 5. send data
    if (send(sock_fd, message_from_cl, strlen(message_from_cl), 0) < 0){
        printf("failed to send message\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }


    // 6. read data from server
    recv_data_descriptor = recv(sock_fd, BUFFER, 1023, 0);
    if (recv_data_descriptor < 0){
        printf("data retrival is failed\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    else if(recv_data_descriptor == 0){
        printf("connection closed by server\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("received data from server is -> %s\n", BUFFER);
    close(sock_fd);
    exit(EXIT_SUCCESS);

    return 0;
}
