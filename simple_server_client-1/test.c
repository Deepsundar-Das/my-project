#include <pthread.h>
// ... keep your existing headers ...

void *receive_handler(void *socket_desc) {
    int sock = *(int *)socket_desc;
    char buffer[1024];
    int bytes_received;

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("\nServer disconnected.\n");
            exit(EXIT_SUCCESS);
        }
        printf("\nserver -> %s\n", buffer);
    }
    return NULL;
}

// Inside main(), right after connect() succeeds:
pthread_t recv_thread;
if (pthread_create(&recv_thread, NULL, receive_handler, (void *)&sock_fd) != 0) {
    perror("Could not create thread");
    close(sock_fd);
    exit(EXIT_FAILURE);
}

// Main thread now ONLY handles sending:
char message[1024];
while (1) {
    if (fgets(message, sizeof(message), stdin) != NULL) {
        // Remove trailing newline
        message[strcspn(message, "\n")] = '\0';
        if (send(sock_fd, message, strlen(message), 0) < 0) {
            perror("Send failed");
            break;
        }
    }
}
