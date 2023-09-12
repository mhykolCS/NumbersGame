#include "client.h"
#define NET_PROTOCOL AF_INET
#define TCP_CONNECTION SOCK_STREAM
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){

    if(argc < 4){
        printf("Correct usage: game_client.exe \"game\" \"server name\" \"port\"\n");
        exit(1);
    }

    int PORT = atoi(argv[3]);
    int client_socket;
    int server_socket;
    int recv_bytes;
    char game_number[2];
    char first_word[10];
    char client_message[BUFFER_SIZE];
    char server_message[BUFFER_SIZE];

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

    do{
        memset(client_message, '\0', sizeof(client_message));
        memset(server_message, '\0', sizeof(server_message));
        memset(first_word, '\0', sizeof(first_word));
        fflush(stdout);
        fflush(stdin);


        recv_bytes = recv(client_socket, server_message, BUFFER_SIZE-1, 0);

        if(recv_bytes == 0){
            continue;
        }
        if(server_message[0] != '\0'){
            for(int i = 0; i < 10; i++){
                if(server_message[i] == ' '){
                    strncpy(first_word, server_message, i);
                    break;                 
                }
            }
        }

        if(strcmp(first_word, "TEXT") == 0){
            printf("%s\n", &server_message[5]);
        }

        if(strcmp(first_word, "GO") == 0){
            printf("It is your turn!, the current score is %s\n", &server_message[3]);
            scanf("%s", game_number);
            if(strcmp(client_message, "quit") == 0){
                strcat(client_message, " ");
                send(client_socket, client_message, sizeof(client_message), 0);
                break;
            }
            memset(client_message, '\0', sizeof(client_message));
            strcpy(client_message, "MOVE ");
            strcat(client_message, game_number);
            send(client_socket, client_message, sizeof(client_message), 0);
        }

        if(strcmp(first_word, "END") == 0){
            printf("Your opponent has left the game\n");
            exit(0);
        }

        if(strcmp(first_word, "ERROR") == 0){
            printf("%s\n", &server_message[6]);
        }

    }while(strcmp(client_message, "quit") != 0);

    close(client_socket);
    close(server_socket);

    return 0;
}