#include "client.h"

int main(int argc, char const *argv[])
{
    /* Parameters for connection */
    int socket;
    string query; // filled by client-side application

    /* Parameters for client application */
    bool isStillWorking = true;
    int choice;
    string clientOption;
    string data;
    char buffer[BUFFER_SIZE] = {0}; // filled by server-side application


    do
    {
        // Establish connection with server.
        socket = ServerConnection();
        if(socket < 0){
            perror("Socket ERROR");
        }

        ShowMainMenu();
        choice = GetUserChoice();
        switch (choice)
        {
        case 0:
            // Quit program.
            cout << "Program ended. Please, wait..." << endl;
            isStillWorking = false;
            shutdown(socket, 2);
            break;

        case 1:
            clientOption = "1";
            cout << "Enter title: ";
            data = GetData(true);
            query = clientOption + "|" + data;
            send(socket, query.c_str(), sizeof(query), 0);
            recv(socket, buffer, sizeof(query), 0);
            if (buffer[0] == '1')
            {
                cout << "This book is in archive." << endl;
            }
            else
            {
                cout << "This book isn't in archive." << endl;
            }
            break;

        case 2:
            clientOption = "2";
            cout << "Enter title: ";
            data = GetData(true);
            query = clientOption + "|" + data;
            send(socket, query.c_str(), sizeof(query), 0);
            recv(socket, buffer, sizeof(query), 0);
            if (buffer[0] == '1')
            {
                cout << "Book rented." << endl;
            }
            else
            {
                cout << "Can't rent book." << endl;
            }
            break;

        case 3:
            clientOption = "3";
            cout << "Enter title: ";
            data = GetData(true);
            query = clientOption + "|" + data;
            send(socket, query.c_str(), sizeof(query), 0);
            recv(socket, buffer, sizeof(query), 0);
            if (buffer[0] == '1')
            {
                cout << "Book returned." << endl;
            }
            else
            {
                cout << "Book not returned." << endl;
            }
            break;
        }
    } while (isStillWorking);
    return 0;
}

/**
 * @brief Show main menu (client-side)
 * 
 */
void ShowMainMenu()
{
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
int GetUserChoice()
{
    int input;
    bool isValid = false;
    do
    {
        cout << "Enter your choice: ";
        cin >> input;
        if (input >= 0 && input <= 5)
        {
            isValid = true;
        }
        else
        {
            cout << "Try again...." << endl;
        }
    } while (!isValid);
    return input;
}

/**
 * @brief Get the Data object
 * 
 * @param dumpFlag TRUE if the buffer must be dumped.
 * @return string - Final string.
 */
string GetData(bool dumpFlag)
{
    string dump;
    string data;
    if (dumpFlag)
    {
        getline(cin, dump);
    }
    getline(cin, data);
    return data;
}

/**
 * @brief Establish connection with server
 * 
 * @return int Connection socket for client
 */
int ServerConnection()
{
    int connectionSocket;
    struct sockaddr_in serverAddress;
    if ((connectionSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);
    int result = inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr);
    if (result <= 0)
    {
        perror("Buffer");
        exit(EXIT_FAILURE);
    }
    if (connect(connectionSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Connection");
        exit(EXIT_FAILURE);
    }
    return connectionSocket;
}