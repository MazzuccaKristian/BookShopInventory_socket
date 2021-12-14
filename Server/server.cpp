#include "server.h"

int main(int argc, char const *argv[]){
    int serverSocket;
    int socketOption = 1;
    struct sockaddr_in socketAddress;

    // Socket creation: IPv4, UDP and IP protocol.
    if(serverSocket = socket(AF_INET, SOCK_STREAM, 0) < 0){
        perror("Socket creation");
        exit(EXIT_FAILURE);
    }

    // Socket setup: reusable address and port for single socket.
    if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketOption, sizeof(socketOption))){
        perror("Socket setup");
        exit(EXIT_FAILURE);
    }
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(PORT);

    // Socket binding.
    if(bind(serverSocket, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) < 0){
        perror("Socket binding");
        exit(EXIT_FAILURE);
    }

    // Accept 1 connection. PROTOTYPE.
    if(listen(serverSocket, CONNECTIONS) < 0){
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    return 0;
}