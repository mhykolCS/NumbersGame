#include "client.h"
#define NET_PROTOCOL AF_INET
#define TCP_CONNECTION SOCK_STREAM

int main(int argc, char *argv[]){

    if(argc < 4){
        printf("Correct usage: game_client.exe \"game\" \"server name\" \"port\"\n");
        exit(1);
    }

    int PORT = atoi(argv[3]);
    int client_socket;
    int server_socket;

    // fill const client struct with data
    const struct sockaddr_in client_info = (const struct sockaddr_in){
        .sin_family = NET_PROTOCOL,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };

    // create socket
    client_socket = socket(NET_PROTOCOL, TCP_CONNECTION, 0);

    if(client_socket < 0){
        printf("Socket creation failed, exiting..\n");
        close(client_socket);
        exit(1);
    } else{
        printf("TCP Socket successfully created\n");
    }

    // connect to game server

    server_socket = connect(client_socket, (struct sockaddr *) &client_info, sizeof(client_info));
    if(server_socket == -1){
        printf("Connection to server failed, exiting..\n");
        close(server_socket);
        close(client_socket);
        exit(1);
    }else{
        printf("Connected to server\n\n");
    }

    close(client_socket);
    close(server_socket);

    return 0;
}