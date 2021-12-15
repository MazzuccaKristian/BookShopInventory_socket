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

    // Receive option value from client.
    received = recv(newSocket, buffer, sizeof(buffer), 0);
    if(received < 0){
        perror("Read");
    }
    int option = std::stoi(buffer);
    switch(option){
        case 1:
            // PROTOTYPE: get first record from archive, then send it to client.
            string archive;
            ShowArchive(&archive);
            const char* formattedArchive = archive.c_str();
            send(newSocket, formattedArchive, sizeof(formattedArchive), 0);
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

/**
 * @brief Read single record from archive.
 *        PROTOTYPE.
 * 
 * @param archive string pointer
 */
void ShowArchive(string* archive){
    ifstream books (BOOKS);
    if(books.is_open()){
        archive -> clear();
        string readed;
        books >> readed;
        archive -> insert(0, readed);
    }
    books.close();
}