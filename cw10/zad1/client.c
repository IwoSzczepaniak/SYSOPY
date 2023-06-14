#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int socket_fd;

void handle_interruption(int signum) {
    printf("Interrupt signal received. Shutting down...\n");

    // Sending the STOP command to the server
    const char *stop_cmd = "STOP";
    send(socket_fd, stop_cmd, strlen(stop_cmd), 0);

    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <name> <connection_type> <server_address>\n", argv[0]);
        return 1;
    }

    // const char* name = argv[1];
    const char* connection_type = argv[2];
    const char* server_address = argv[3];

    // Initializing the interruption handler
    signal(SIGINT, handle_interruption);

    if (strcmp(connection_type, "network") == 0) {
        // Creating a network socket
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1) {
            perror("Socket creation failed");
            return 1;
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(12345);

        // Converting the server address to binary form
        if (inet_pton(AF_INET, server_address, &(server_addr.sin_addr)) <= 0) {
            perror("Invalid server address");
            return 1;
        }

        // Establishing connection with the server
        if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            perror("Connection failed");
            return 1;
        }
    }
    else if (strcmp(connection_type, "local") == 0) {
        // Creating a local socket
        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (socket_fd == -1) {
            perror("Local socket creation failed");
            return 1;
        }

        struct sockaddr_un server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sun_family = AF_UNIX;
        strncpy(server_addr.sun_path, server_address, sizeof(server_addr.sun_path) - 1);

        // Establishing connection with the server
        if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            perror("Connection failed");
            return 1;
        }
    }
    else {
        printf("Invalid connection type\n");
        return 1;
    }

    while (1) {
        // Sending commands to the server
        char command[BUFFER_SIZE];
        memset(command, 0, sizeof(command));
        fgets(command, sizeof(command), stdin);

        if (strncmp(command, "STOP", 4) == 0) {
            // Sending the STOP command to the server
            send(socket_fd, command, strlen(command), 0);
            break;
        }


        // write(socket_fd, command, strlen(command));
        send(socket_fd, command, strlen(command) - 1, 0);

        printf("Sent command to server: %s\n", command);

        // Receiving response from the server
        char response[BUFFER_SIZE];
        memset(response, 0, sizeof(response));
        ssize_t bytes_received = recv(socket_fd, response, sizeof(response) - 1, 0);
        if (bytes_received == -1) {
            perror("Receiving response failed");
            break;
        }

        if (strncmp(response, "SERVER_STOPPED", 4) == 0) {
            printf("Received the STOP command from the server\n");
            break;
        }

        printf("Response from server: %s\n", response);
    }

    // Closing the connection
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);

    return 0;
}
