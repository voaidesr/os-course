#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define BUF_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main(void) {
    int sock_fd;
    struct sockaddr_in serv_addr;
    char send_buffer[BUF_SIZE];
    char recv_buffer[BUF_SIZE];
    ssize_t n;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Eroare la socket()");
        exit(1);
    }

    // format for ipv4
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT); // sets right endian order

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Eroare la inet_pton() / Adresă invalidă");
        close(sock_fd);
        exit(1);
    }

    printf("Încercare conectare la %s:%d...\n", SERVER_IP, PORT);
    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Eroare la connect()");
        close(sock_fd);
        exit(1);
    }

    printf("Conectat la server! (CTRL+D sau 'exit' pentru oprire).\n"); //ctrl-d gives EOF

    // main loop
    while (fgets(send_buffer, BUF_SIZE, stdin) != NULL) {

        if (strncmp(send_buffer, "exit", 4) == 0) {
            break;
        }

        if (send(sock_fd, send_buffer, strlen(send_buffer), 0) < 0) {
            perror("Eroare la send()");
            break;
        }

        n = recv(sock_fd, recv_buffer, BUF_SIZE - 1, 0);
        if (n < 0) {
            perror("Eroare la recv()");
            break;
        } else if (n == 0) {
            printf("Serverul a închis conexiunea.\n");
            break;
        }

        recv_buffer[n] = '\0';
        printf("Server Echo: %s", recv_buffer);
    }

    close(sock_fd);

    return 0;
}