#include "server.h"

int main(int argc, char const *argv[]){
    int serverSocket, newSocket;
    int socketOption = 1;
    struct sockaddr_in socketAddress;
    int addressLength = sizeof(socketAddress);

    int received;
    char buffer[2048] = {0};

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

    // Receive option value and data (if needed) from client.
    received = recv(newSocket, buffer, BUFFER_SIZE, 0);
    if(received <= 0){
        perror("RECV");
    }else{
        string dataFromClient = buffer;
        int option = std::stoi(dataFromClient.substr(0, 2));
        switch(option){
            case 1:
                string userData = dataFromClient.substr(dataFromClient.find_first_of("|") + 1);
                string username, password;
                username = userData.substr(0, userData.find_first_of("|"));
                password = userData.substr(userData.find_first_of("|") + 1);
                RecordNewUser(username, password);
                break;
        }
    }

    return 0;
}

/**
 * @brief Setup file (archive and users).
 * 
 */
void Setup(){
    ifstream books;
    ifstream users;
    books.open(BOOKS);
    if(!books.is_open()){
        // Here if the file does NOT exist
        ofstream newFile;
        newFile.open(BOOKS);
        if(newFile.is_open()){
            newFile.close();
        }
    }
    users.open(USERS);
    if(!users.is_open()){
        ofstream usersTemp;
        usersTemp.open(USERS);
        if(usersTemp.is_open()){
            usersTemp.close();
        }
    }
    books.close();
    users.close();
}

void RecordNewUser(string name, string ppww){
    ofstream users;
    users.open(USERS, ofstream::app);
    if(users.is_open()){
        string user = "User: " + name + " " + "Password: " + ppww;
        users << user << endl;
    }
    users.close();
}