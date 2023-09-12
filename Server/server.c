#include "server.h"
#define NET_PROTOCOL AF_INET
#define TCP_CONNECTION SOCK_STREAM
#define PORT 80
#define BACKLOG 10

int main(int argc, char *argv[]){

    /*if(argc < 4){
        printf("Correct usage: game_server.exe \"port\" \"game\" \"game arguments\"\n");
        exit(1);
    }*/

    struct sockaddr_in client_info;
    int server_socket;
    int server_connection;
    int server_bind_status;
    int server_listen_status;
    int client_socket;
    int client_socket_size = sizeof(client_info);


    fd_set current_sockets, ready_sockets;

    // TCP Socket
    server_socket = socket(NET_PROTOCOL, TCP_CONNECTION, 0);
    if(server_socket == -1){
        printf("Socket creation failed, exiting..\n");
        close(server_socket);
        exit(1);
    }else{
        printf("TCP Socket successfully created\n");
    }

    // fill const server struct with data
    
    const struct sockaddr_in server_info = (const struct sockaddr_in){
        .sin_family = NET_PROTOCOL,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };

    // bind socket to port
    server_bind_status = bind(server_socket, (const struct sockaddr*)&server_info, sizeof(server_info));
    if(server_bind_status < 0){
        printf("Failed to bind to port, exiting..\n");
        close(server_socket);
        exit(1);
    }else{
        printf("Bind successful\n");
    }

    // listen for connections
    server_listen_status = listen(server_socket, BACKLOG);
    if(server_listen_status != 0){
        printf("Listening on port 80 failed, exiting..\n");
        close(server_socket);
        exit(1);
    }else{
        printf("Listening on port 80..\n");
    }

    // accept connection request

    client_socket = accept(server_socket, (struct sockaddr *)&client_info, &client_socket_size);
    if(client_socket < 0){
        printf("Accept request failed, exiting..\n");
        close(server_socket);
        close(client_socket);
        exit(1);
    }else{
        printf("Connection Accepted\n");
        active_connections++;
    }

    close(server_socket);

    return 0;
}