#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAX_CLIENTS 20
#define BUFFER_SIZE 1024

typedef struct {
    int socket_fd;
    char name[20];
} Client;

int num_clients = 0;
Client clients[MAX_CLIENTS];
int server_socket_fd;
int local_socket_fd;
int epoll_fd;
const char* init_message = "Welcome to the server!";
const char* max_clients_message = "Maximum number of clients reached. Try again later.\n";



void handle_interruption(int signum) {
    printf("Interrupt signal received. Shutting down...\n");

    // Sending STOP signal to all clients
    for (int i = 0; i < num_clients; i++) {
        const char* stop_message = "SERVER_STOPPED";
        send(clients[i].socket_fd, stop_message, strlen(stop_message), 0);
    }

    // Closing client sockets
    for (int i = 0; i < num_clients; i++) {
        close(clients[i].socket_fd);
    }

    exit(0);
}

void handle_client_message(int client_index, char* message) {
    // Handling commands from the client
    printf("Received message from client %s: %s\n", clients[client_index].name, message);

    if (strncmp(message, "LIST", 4) == 0) {
        // Sending the list of active clients to the client
        char client_list[MAX_CLIENTS * 20];  // Buffer for the client list
        memset(client_list, 0, sizeof(client_list));
        for (int i = 0; i < num_clients; i++) {
            strcat(client_list, clients[i].name);
            strcat(client_list, "\n");
        }
        send(clients[client_index].socket_fd, client_list, strlen(client_list), 0);
    }
    else if (strncmp(message, "2ALL", 4) == 0) {
        // Forwarding the message to all clients
        for (int i = 0; i < num_clients; i++) {
            if (i != client_index) {
                char full_message[BUFFER_SIZE + 30];  // Buffer for the message with the client sender's identifier and date
                memset(full_message, 0, sizeof(full_message));
                sprintf(full_message, "%s: %s", clients[client_index].name, message + 4);
                send(clients[i].socket_fd, full_message, strlen(full_message), 0);
            }
        }
    }
    else if (strncmp(message, "2ONE", 4) == 0) {
        // Forwarding the message to a specific client
        int recipient_id = atoi(message + 5);
        if (recipient_id >= 0 && recipient_id < num_clients) {
            char full_message[BUFFER_SIZE + 30];  // Buffer for the message with the client sender's identifier and date
            memset(full_message, 0, sizeof(full_message));
            sprintf(full_message, "%s: %s", clients[client_index].name, message + 7);
            send(clients[recipient_id].socket_fd, full_message, strlen(full_message), 0);
        }
        else printf("Invalid client ID received from client\n");
        
    }
    else if (strncmp(message, "STOP", 4) == 0) {
        // Removing the client from the client list
        // Closing the client socket
        close(clients[client_index].socket_fd);
        // Moving the remaining clients in the array to fill the gap
        for (int i = client_index; i < num_clients - 1; i++) {
            clients[i] = clients[i + 1];
        }
        num_clients--;
    }
    else 
    {
        printf("Invalid command received from client %s\n", clients[client_index].name);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <port> <socket_path> <connection_type>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    const char* socket_path = argv[2];
    const char* connection_type = argv[3];

    // Initializing the interruption handler
    signal(SIGINT, handle_interruption);

    // Initializing the epoll mechanism
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Epoll creation failed");
        return 1;
    }
    struct epoll_event event;



    if (strcmp(connection_type, "network") == 0) {
        // Creating a network socket
        server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket_fd == -1) {
            perror("Socket creation failed");
            return 1;
        }

        // Setting the address for the network socket
        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons(port);

        // Binding the network socket to the port
        if (bind(server_socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
            perror("Bind failed");
            return 1;
        }

        // Listening on the network socket
        if (listen(server_socket_fd, MAX_CLIENTS) == -1) {
            perror("Listen failed");
            return 1;
        }

        event.events = EPOLLIN;
        event.data.fd = server_socket_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &event) == -1) {
            perror("Epoll control failed for local socket");
            return 1;
        }
    }

    else if (strcmp(connection_type, "local") == 0) {
        // Creating a local socket
        local_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (local_socket_fd == -1) {
            perror("Local socket creation failed");
            return 1;
        }

        // Setting the address for the local socket
        struct sockaddr_un local_address;
        memset(&local_address, 0, sizeof(local_address));
        local_address.sun_family = AF_UNIX;
        strncpy(local_address.sun_path, socket_path, sizeof(local_address.sun_path) - 1);

        // Binding the local socket to the path
        if (bind(local_socket_fd, (struct sockaddr*)&local_address, sizeof(local_address)) == -1) {
            perror("Local bind failed");
            return 1;
        }

        // Listening on the local socket
        if (listen(local_socket_fd, MAX_CLIENTS) == -1) {
            perror("Local listen failed");
            return 1;
        }

        event.events = EPOLLIN;
        event.data.fd = local_socket_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, local_socket_fd, &event) == -1) {
            perror("Epoll control failed for local socket");
            return 1;
        }
    }
    else {
        printf("Invalid connection type\n");
        return 1;
    }

    struct epoll_event events[MAX_CLIENTS+2];
    char buffer[BUFFER_SIZE];

    while (1) {
        int num_events = epoll_wait(epoll_fd, events, MAX_CLIENTS + 2, 1);
        if (num_events == -1) {
            perror("Epoll wait failed");
            break;
        }


        for (int i = 0; i < num_events; i++) 
        {
            if (events[i].data.fd == server_socket_fd) 
            {
                // New connection on the network socket
                int client_socket_fd = accept(server_socket_fd, NULL, NULL);
                if (client_socket_fd == -1) {
                    perror("Accept failed");
                    continue;
                }

                
                // Adding the client to the clients list
                if (num_clients < MAX_CLIENTS) {
                    
                    clients[num_clients].socket_fd = client_socket_fd;
                    num_clients++;
                    printf("New client connected\n");
                    send(client_socket_fd, init_message, strlen(init_message), 0);
                }
                else {
                    send(client_socket_fd, max_clients_message, strlen(max_clients_message), 0);
                    close(client_socket_fd);
                }
            }
            else if (events[i].data.fd == local_socket_fd) 
            {
                // New connection on the local socket
                struct sockaddr_un client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int client_socket_fd = accept(local_socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_socket_fd == -1) {
                    perror("Accept failed");
                    continue;
                }

                // Adding the client to the clients list
                if (num_clients < MAX_CLIENTS) {
                    
                    clients[num_clients].socket_fd = client_socket_fd;
                    num_clients++;
                    printf("New client connected\n");
                    send(client_socket_fd, init_message, strlen(init_message), 0);
                }
                else {
                    send(client_socket_fd, max_clients_message, strlen(max_clients_message), 0);
                    close(client_socket_fd);
                }
            }

            else
            {
                printf("Message from client\n");
                // Data received from a client
                int client_socket_fd = events[i].data.fd;
                int num_bytes = recv(client_socket_fd, buffer, BUFFER_SIZE, 0);
                if (num_bytes > 0) {
                    buffer[num_bytes] = '\0';
                    
                    for (int j = 0; j < num_clients; j++) {
                        if (clients[j].socket_fd == client_socket_fd) {
                            if(read(client_socket_fd, buffer, BUFFER_SIZE) == -1) perror("Error receiving message");
                            else 
                            {
                                printf("Message from client \"%s\"\n",buffer);
                                handle_client_message(i, buffer);
                            }
                            break;
                        }
                    }
                    
                }
                else {
                    // Client disconnected
                    for (int j = 0; j < num_clients; j++) {
                        if (clients[j].socket_fd == client_socket_fd) {
                            printf("Client disconnected: %s\n", clients[j].name);
                            clients[j] = clients[num_clients - 1];
                            num_clients--;
                            break;
                        }
                    }
                    close(client_socket_fd);
                }
            }
            
        }
    }

    // Closing the sockets
    close(server_socket_fd);
    close(local_socket_fd);

    // Removing the epoll descriptor
    close(epoll_fd);

    return 0;
}
