#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // Replace with your server's IP
#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // Create a socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection to server failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read input from the user
        printf("Client 2: \n");
        //fgets(buffer, sizeof(buffer), stdin);

        // Send the input to the server
        // send(client_socket, buffer, strlen(buffer), 0);

        // Receive and print the server's response
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0) {
            perror("Server disconnected");
            break;
        }
        printf("Server: %s\n", buffer);
    }

    close(client_socket);

    return 0;
}