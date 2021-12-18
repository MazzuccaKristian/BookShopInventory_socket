#include "server.h"

int main(int argc, char const *argv[]){
    int serverSocket, newSocket;
    int socketOption = 1;
    struct sockaddr_in socketAddress;
    int addressLength = sizeof(socketAddress);

    int received;
    char buffer[2048] = {0};

    string searchedTitle;
    string queryForClient;

    bool isTitleFound;

    string userData;
    string username, password;

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
    if(received < 0){
        perror("RECV");
    }else{
        string dataFromClient = buffer;
        int option = std::stoi(dataFromClient.substr(0, 2));
        switch(option){
            case 1:
                searchedTitle = dataFromClient.substr(dataFromClient.find_first_of("|") + 1);
                isTitleFound = SearchBook(searchedTitle);
                if(isTitleFound){
                    queryForClient = "1";
                }else{
                    queryForClient = "0";
                }
                send(newSocket, queryForClient.c_str(), sizeof(queryForClient), 0);
                break;

            case 2:
                searchedTitle = dataFromClient.substr(dataFromClient.find_first_of("|") + 1);
                isTitleFound = SearchBook(searchedTitle);
                if(isTitleFound){
                    if(RentBook(searchedTitle)){
                        queryForClient = "1";
                    }else{
                        queryForClient = "0";
                    }
                }else{
                    queryForClient = "0";
                }
                send(newSocket, queryForClient.c_str(), sizeof(queryForClient), 0);
                break;

            case 3:
                searchedTitle = dataFromClient.substr(dataFromClient.find_first_of("|") + 1);
                if(UpdateBooks(searchedTitle)){
                    queryForClient = "1";
                }else{
                    queryForClient = "0";
                }
                send(newSocket, queryForClient.c_str(), sizeof(queryForClient), 0);
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

/**
 * @brief Search if a book is present in archive.
 * 
 * @param title Searched title
 * @return true if the book is available
 * @return false if the book isn't available
 */
bool SearchBook(string title){
    ifstream books;
    bool found = false;
    books.open(BOOKS);
    if(books.is_open()){
        string titleKey, titleValue, copyKey, copyValue;
        while(books >> titleKey >> titleValue >> copyKey >> copyValue){
            if((titleKey.compare("Title:") == 0) && (titleValue.compare(title) == 0)){
                found = true;
            }
        }
    }
    books.close();
    return found;
}

/**
 * @brief Search a book in archive and decrement the number of copies available.
 * 
 * @param title Searched title
 * @return true if the book is in archive with at least one copy.
 * @return false 
 */
bool RentBook(string title){
    bool rented = false;
    int copies;
    ofstream tempFile;
    ifstream books;
    books.open(BOOKS);
    tempFile.open(TEMP);
    if(books.is_open() && tempFile.is_open()){
        string titleKey, titleValue, copyKey, copyValue;
        while(books >> titleKey >> titleValue >> copyKey >> copyValue){
            if((titleKey.compare("Title:") == 0) && (titleValue.compare(title) == 0)){
                // Here if the book is in archive.
                copies = std::stoi(copyValue);
                if(copies > 0){
                    copies--;
                    rented = true;
                }
            }
            string record = titleKey + " " + titleValue + " " + copyKey + " " + std::to_string(copies);
            tempFile << record << endl;
        }
    }
    tempFile.close();
    books.close();
    remove(BOOKS);
    rename(TEMP, BOOKS);
    return rented;
}

/**
 * @brief Update (increment) number of copies of a book.
 * 
 * @param title title of the book that must be updated in archive.
 * @return true if the book has been updated correctly.
 * @return false 
 */
bool UpdateBooks(string title){
    bool updated = false;
    bool added = false;
    ifstream books;
    ofstream tempFile;
    int copies;
    string record;
    books.open(BOOKS);
    tempFile.open(TEMP);
    if(books.is_open() && tempFile.is_open()){
        string titleKey, titleValue, copyKey, copyValue;
        while(books >> titleKey >> titleValue >> copyKey >> copyValue){
            if((titleKey.compare("Title:") == 0) && (titleValue.compare(title) == 0)){
                copies = std::stoi(copyValue);
                copies++;
                updated = true;
                added = true;
                record = titleKey + " " + titleValue + " " + copyKey + " " + std::to_string(copies);
            }else{
                record = titleKey + " " + titleValue + " " + copyKey + " " + copyValue;
            }
            tempFile << record << endl;
        }
        if(!added){
            record = titleKey + " " + title + " " + copyKey + " 1";
            tempFile << record << endl;
            added = true;
        }
    }
    books.close();
    tempFile.close();
    remove(BOOKS);
    rename(TEMP, BOOKS);
    return added;
}