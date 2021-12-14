#include "client.h"

int main(int argc, char const *argv[]){
    int server = 0;
    struct sockaddr_in serverAddress;

    // Socket creation: IPv4, UDP and IP protocol.
    if((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket creation");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    int result = inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
    if(result == 0){
        perror("Invalid buffer");
        exit(EXIT_FAILURE);
    }else if(result < 0){
        perror("AF error");
        exit(EXIT_FAILURE);
    }

    if(connect(server, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        perror("Connection");
        exit(EXIT_FAILURE);
    }

    return 0;
}