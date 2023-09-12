#include "server.h"
#define NET_PROTOCOL AF_INET
#define TCP_CONNECTION SOCK_STREAM
#define PORT 80
#define BACKLOG 10

int main(){

    int server_socket;
    int server_connection;
    int server_bind_status;
    int server_listen_status;

    //TCP Socket
    server_socket = socket(NET_PROTOCOL, TCP_CONNECTION, 0);
    if(server_socket == -1){
        printf("Socket Creation Failed, exiting..\n");
        close(server_socket);
        exit(1);
    }else{
        printf("TCP Socket Successfully Created\n");
    }

    //Fill server struct with data and make const
    const struct sockaddr_in server_info = (struct sockaddr_in){
        .sin_family = NET_PROTOCOL,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };

    //bind socket to port
    server_bind_status = bind(server_socket, (const struct sockaddr*)&server_info, sizeof(server_info));
    if(server_bind_status < 0){
        printf("Failed to bind to port, exiting..\n");
        close(server_socket);
        exit(1);
    }else{
        printf("Bind successful\n");
    }

    server_listen_status = listen(server_socket, BACKLOG)






    close(server_socket);

    return 0;
}