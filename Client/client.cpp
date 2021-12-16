#include "client.h"

int main(int argc, char const *argv[]){
    // Connection params
        int server = 0;
        struct sockaddr_in serverAddress;
    //
    // Application params
        bool isStillWorking = true;
        int choice;
        char buffer[2048] = {0};
        int dataFromServer;
        string clientOption;
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
                    // Retrieve data from client
                    string username, password, user;
                    clientOption = "1";
                    cin.ignore();
                    cout << "Enter your username: ";
                    getline(cin, username);
                    cout << "Enter your password: ";
                    getline(cin, password);
                    user = clientOption + "|" + username + "|" + password;
                    const char* formattedUser = user.c_str();
                    // Send data to Server.
                    send(server, formattedUser, strlen(formattedUser), 0);
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
    cout << "1. Create new account;" << endl;
    cout << "2. Login;" << endl;
    cout << "3. Search book;" << endl;
    cout << "4. Rent book;" << endl;
    cout << "5. Return book;" << endl;
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
        if(input >= 0 && input <= 1){
            isValid = true;
        }else{
            cout << "Try again...." << endl;
        }
    }while(!isValid);
    return input;
}