#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket1, client_socket2;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //unlink();
    
    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 2) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept the first client
    if ((client_socket1 = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
        perror("Acceptance of client 1 failed");
        exit(EXIT_FAILURE);
    }
    printf("Client 1 connected\n");

    // Accept the second client
    if ((client_socket2 = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
        perror("Acceptance of client 2 failed");
        exit(EXIT_FAILURE);
    }
    printf("Client 2 connected\n");

    while (1) {
        // Receive from client 1 and send to client 2
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_socket1, buffer, sizeof(buffer), 0) <= 0) {
            perror("Client 1 disconnected");
            break;
        }
        printf("Client 1: %s", buffer);
        send(client_socket2, buffer, strlen(buffer), 0);

    //     // Receive from client 2 and send to client 1
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_socket2, buffer, sizeof(buffer), 0) <= 0) {
            perror("Client 2 disconnected");
            break;
        }
        printf("Client 2: %s", buffer);
        send(client_socket1, buffer, strlen(buffer), 0);
    }

    close(server_socket);
    close(client_socket1);
    close(client_socket2);

    return 0;
}
