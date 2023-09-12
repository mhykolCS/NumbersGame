#include "server.h"
#define NET_PROTOCOL AF_INET
#define TCP_CONNECTION SOCK_STREAM
#define PORT 80
#define BACKLOG 10
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){

    /*if(argc < 4){
        printf("Correct usage: game_server.exe \"port\" \"game\" \"game arguments\"\n");
        exit(1);
    }*/

    struct sockaddr_in client_info_one;
    struct sockaddr_in client_info_two;
    int server_socket;
    int server_connection;
    int server_bind_status;
    int server_listen_status;
    int client_socket_one;
    int client_socket_two;
    int client_socket_size = sizeof(client_info_one);
    int game_score = 0;
    int game_recieved_number;
    int game_current_turn = 1;
    int current_client_socket;
    char client_message[BUFFER_SIZE];
    char server_message[BUFFER_SIZE];
    char first_word[10];
    char game_score_char[2];
    memset(first_word, '\0', sizeof(first_word));
    memset(client_message, '\0', sizeof(client_message));
    memset(server_message, '\0', sizeof(server_message));

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

    client_socket_one = accept(server_socket, (struct sockaddr *)&client_info_one, &client_socket_size);
    if(client_socket_one < 0){
        printf("Accept request failed, exiting..\n");
        close(server_socket);
        close(client_socket_one);
        exit(1);
    }else{
        printf("Player One Connection Accepted\n");
        strcpy(server_message, "TEXT Welcome to the game Player One!");
        send(client_socket_one, server_message, sizeof(server_message), 0);
    }

    client_socket_two = accept(server_socket, (struct sockaddr*)&client_info_two, &client_socket_size);
    if(client_socket_two < 0){
        printf("Accept request failed, exiting..\n");
        close(server_socket);
        close(client_socket_one);
        close(client_socket_two);
    }else{
        printf("Player Two Connection Accepted\n");
        strcpy(server_message, "TEXT Welcome to the game Player Two!");
        send(client_socket_two, server_message, sizeof(server_message), 0);
    }

    do{
        memset(client_message, '\0', sizeof(client_message));
        memset(server_message, '\0', sizeof(server_message));
        memset(first_word, '\0', sizeof(first_word)); 

        if(game_current_turn == 1){
                current_client_socket = client_socket_one;
                game_current_turn = 2;
            }else{
                current_client_socket = client_socket_two;
                game_current_turn = 1;
            }


        sprintf(game_score_char, "%d", game_score);

        strcpy(server_message, "GO ");
        strcat(server_message, game_score_char);
        send(current_client_socket, server_message, sizeof(server_message), 0);

        recv(current_client_socket, client_message, BUFFER_SIZE-1, 0);

        if(client_message[0] != '\0'){
            for(int i = 0; i < 10; i++){
                if(client_message[i] == ' '){
                    strncpy(first_word, client_message, i);
                    break;                    
                }
            }
        }


        printf("Here\n");
        if(strcmp(first_word, "MOVE") == 0){
            printf("Here2\n");
            game_recieved_number = atoi(&client_message[3]);
            printf("%d\n", game_recieved_number);
            
        }else if(strcmp(first_word, "QUIT") == 0){
            strcpy(server_message, "TEXT Your opponent has left the game, exiting..");
            if(game_current_turn == 1){
                send(client_socket_one, server_message, sizeof(server_message), 0);
                memset(server_message, '\0', sizeof(server_message));
                strcpy(server_message, "EXIT ");
                send(client_socket_one, server_message, sizeof(server_message), 0);
            }else{
                send(client_socket_two, server_message, sizeof(server_message), 0);
                memset(server_message, '\0', sizeof(server_message));
                strcpy(server_message, "EXIT ");
                send(client_socket_two, server_message, sizeof(server_message), 0);
            }
            break;
        }
        

        

    }while(game_score > 30);

    close(server_socket);
    close(client_socket_one);
    close(client_socket_two);

    return 0;
}