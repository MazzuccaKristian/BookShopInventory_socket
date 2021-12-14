#include "client.h"

int main(int argc, char const *argv[]){
    // Connection params
        int server = 0;
        struct sockaddr_in serverAddress;
    //
    // Application params
        bool isStillWorking = true;
        int choice;
        char* data;
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
                    cout << "Program ended. Please, wait..." << endl;
                    isStillWorking = false;
                    break;

                case 1:
                    data = "1";
                    send(server, data, strlen(data), 0);
                    cout << "DATA SENT" << endl;
                    break;
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
    cout << "1. Show archive;" << endl;
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