#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/my_socket"

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_un serverAddr;
    char buffer[1024];
    socklen_t addr_size;

    // Creazione del socket
    serverSocket = socket(PF_UNIX, SOCK_STREAM, 0);

    // Configurazione del socket
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, SOCKET_PATH);
    unlink(SOCKET_PATH);

    // Bind del socket
    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    // Ascolto sul socket
    listen(serverSocket, 5);

    // Accettazione della connessione
    addr_size = sizeof(serverAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *) &serverAddr, &addr_size);

    // Ricezione del messaggio //POTREBBE ESSERE INIZIO
    recv(clientSocket, buffer, 1024, 0);
    printf("Messaggio ricevuto: %s\n", buffer);

    pid_t pid = fork();

    if(pid == 0){
        // Siamo nel processo figlio
        // Compiliamo il file sorgente C in un eseguibile e poi lo eseguiamo
        execl("/bin/sh", "sh", "-c", "gcc -o hello HelloWord.c && ./hello", NULL);
    } else if (pid > 0) {
        wait(NULL);
        printf("Sono il padre\n");
    } else {
        // Errore nel fork
        printf("Errore nel fork\n");
        return 1;
    }

    printf("Ciao che?");

    // Chiudere i socket
    close(clientSocket);
    close(serverSocket);

    return 0;
}
