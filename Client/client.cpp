#include "client.h"

int main(int argc, char const *argv[]){
    // Connection params
        int server = 0;
        struct sockaddr_in serverAddress;
    //
    // Application params
        string data1, data2;
        string query;
        int queryResult;

        bool isStillWorking = true;
        int choice;
        char buffer[2048] = {0};
        int dataFromServer;
        string clientOption;
        string username, password, user;
        string title;
    //

    // Connection
        // Socket creation: IPv4, UDP and IP protocol.
        if((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            perror("Socket creation");
            exit(EXIT_FAILURE);
        }

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(PORT);

        int result = inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr);
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
    //
    // Client side application
        do{
            ShowMainMenu();
            choice = GetUserChoice();
            switch(choice){
                case 0:
                    // Quit program.
                    cout << "Program ended. Please, wait..." << endl;
                    isStillWorking = false;
                    break;

                case 1:
                    clientOption = "1";
                    cout << "Enter title: ";
                    data1 = GetData(true);
                    query = clientOption + "|" + data1;
                    send(server, query.c_str(), QUERY_SIZE, 0);
                    recv(server, buffer, QUERY_SIZE, 0);
                    if(buffer[0] == '1'){
                        cout << "This book is available." << endl;
                    }else{
                        cout << "This book isn't available." << endl;
                    }
                    break;

                case 2:
                    // Login (for user)
                    clientOption = "2";
                    cout << "Enter your username: ";
                    data1 = GetData(true);
                    cout << "Enter your password: ";
                    data2 = GetData(false);
                    query = clientOption + "|" + data1 + "|" + data2;
                    send(server, query.c_str(), QUERY_SIZE, 0);
                    break;
                
                // case 3:
                //     clientOption = "3";
                //     cout << "Enter title: ";
                //     cin.ignore();
                //     getline(cin, title);
                //     query = clientOption + "|" + title;
                //     const char* searchQuery = query.c_str();
            }
        }while(isStillWorking);
    //
    return 0;
}

/**
 * @brief Show main menu (client-side)
 * 
 */
void ShowMainMenu(){
    cout << "MAIN MENU" << endl;
    cout << "1. Search book;" << endl;
    cout << "2. Rent a book;" << endl;
    cout << "3. Return a book;" << endl;
    cout << "0. Exit." << endl;
}

/**
 * @brief Get and validate choice.
 * 
 * @return int Validated input.
 */
int GetUserChoice(){
    int input;
    bool isValid = false;
    do{
        cout << "Enter your choice: ";
        cin >> input;
        if(input >= 0 && input <= 5){
            isValid = true;
        }else{
            cout << "Try again...." << endl;
        }
    }while(!isValid);
    return input;
}

/**
 * @brief Get the Data object
 * 
 * @param dumpFlag TRUE if the buffer must be dumped.
 * @return string - Final string.
 */
string GetData(bool dumpFlag){
    string dump;
    string data;
    if(dumpFlag){
        getline(cin, dump);
    }
    getline(cin, data);
    return data;
}