#include "server.h"

int main(int argc, char const *argv[]){
    int serverSocket, newSocket;
    int socketOption = 1;
    struct sockaddr_in socketAddress;
    int addressLength = sizeof(socketAddress);

    int received;
    char buffer[1024];

    // Socket creation: IPv4, UDP and IP protocol.
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
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

    // Server listening 1 connection.
    if(listen(serverSocket, CONNECTIONS) < 0){
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    if((newSocket = accept(serverSocket, (struct sockaddr *)&socketAddress, (socklen_t *)&addressLength)) < 0){
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    Setup();

    received = read(newSocket, buffer, 1024);
    if(received < 0){
        perror("Read");
    }
    int convertedBuffer = std::stoi(buffer);
    switch(convertedBuffer){
        case 1:
            // ShowArchive();
            break;
    }

    return 0;
}

/**
 * @brief Setup file (archive).
 * 
 */
void Setup(){
    ifstream books;
    books.open(BOOKS);
    if(!books.is_open()){
        // Here if the file does NOT exist
        ofstream newFile;
        newFile.open(BOOKS);
        if(newFile.is_open()){
            newFile.close();
        }
    }
    books.close();
}