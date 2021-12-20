#include "server.h"

int main(int argc, char const *argv[])
{
    /* Parameters for connection */
    int serverSocket;
    struct sockaddr_in serverAddress;
    int newSocket;
    struct sockaddr_storage serverStorage;
    socklen_t addressSize;
    int received;
    char buffer[BUFFER_SIZE] = {0}; // filled by client-side application
    string queryForClient;          // filled by server-side application

    /* Parameters for server application */
    bool isServerRunning = true;
    string dataFromClient;
    int option;
    string searchedTitle; // extracted from buffer

    Setup();

    // Socket creation: IPv4, UDP and IP protocol.
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // Socket binding.
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Socket binding");
        exit(EXIT_FAILURE);
    }

    // Server listening CONNECTIONS connections.
    if (listen(serverSocket, CONNECTIONS) == 0)
    {
        cout << "Server listening." << endl;
    }
    else
    {
        perror("Server listening");
        exit(EXIT_FAILURE);
    }

    while (isServerRunning)
    {
        addressSize = sizeof(serverStorage);

        if ((newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addressSize)) < 0)
        {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        // Receive option value and data (if needed) from client.
        received = recv(newSocket, buffer, BUFFER_SIZE, 0);
        if (received < 0)
        {
            perror("RECV");
        }

        dataFromClient = buffer;
        option = std::stoi(dataFromClient.substr(0, 2));
        switch (option)
        {
        case 1:
            searchedTitle = dataFromClient.substr(dataFromClient.find_first_of("|") + 1);
            if (SearchBook(searchedTitle))
            {
                queryForClient = "1";
            }
            else
            {
                queryForClient = "0";
            }
            send(newSocket, queryForClient.c_str(), sizeof(queryForClient), 0);
            break;

        case 2:
            searchedTitle = dataFromClient.substr(dataFromClient.find_first_of("|") + 1);
            if (SearchBook(searchedTitle))
            {
                if (RentBook(searchedTitle))
                {
                    queryForClient = "1";
                }
                else
                {
                    queryForClient = "0";
                }
            }
            else
            {
                queryForClient = "0";
            }
            send(newSocket, queryForClient.c_str(), sizeof(queryForClient), 0);
            break;

        case 3:
            searchedTitle = dataFromClient.substr(dataFromClient.find_first_of("|") + 1);
            if (UpdateBooks(searchedTitle))
            {
                queryForClient = "1";
            }
            else
            {
                queryForClient = "0";
            }
            send(newSocket, queryForClient.c_str(), sizeof(queryForClient), 0);
            break;
        }

        cout << "Server still UP" << endl;
    }

    return 0;
}

/**
 * @brief Setup file (archive and users).
 * 
 */
void Setup()
{
    ifstream books;
    ifstream users;
    books.open(BOOKS);
    if (!books.is_open())
    {
        // Here if the file does NOT exist
        ofstream newFile;
        newFile.open(BOOKS);
        if (newFile.is_open())
        {
            newFile.close();
        }
    }
    users.open(USERS);
    if (!users.is_open())
    {
        ofstream usersTemp;
        usersTemp.open(USERS);
        if (usersTemp.is_open())
        {
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
bool SearchBook(string title)
{
    ifstream books;
    bool found = false;
    books.open(BOOKS);
    if (books.is_open())
    {
        string titleKey, titleValue, copyKey, copyValue;
        while (books >> titleKey >> titleValue >> copyKey >> copyValue)
        {
            if ((titleKey.compare("Title:") == 0) && (titleValue.compare(title) == 0))
            {
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
bool RentBook(string title)
{
    bool rented = false;
    int copies;
    string record;
    ofstream tempFile;
    ifstream books;
    books.open(BOOKS);
    tempFile.open(TEMP);
    if (books.is_open() && tempFile.is_open())
    {
        string titleKey, titleValue, copyKey, copyValue;
        while (books >> titleKey >> titleValue >> copyKey >> copyValue)
        {
            if ((titleKey.compare("Title:") == 0) && (titleValue.compare(title) == 0))
            {
                // Here if the book is in archive.
                copies = std::stoi(copyValue);
                if (copies > 0)
                {
                    copies = copies - 1;
                    rented = true;
                    copyValue = std::to_string(copies);
                }
            }
            record = titleKey + " " + titleValue + " " + copyKey + " " + copyValue;
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
bool UpdateBooks(string title)
{
    bool updated = false;
    bool added = false;
    ifstream books;
    ofstream tempFile;
    int copies;
    string record;
    books.open(BOOKS);
    tempFile.open(TEMP);
    if (books.is_open() && tempFile.is_open())
    {
        string titleKey, titleValue, copyKey, copyValue;
        while (books >> titleKey >> titleValue >> copyKey >> copyValue)
        {
            if ((titleKey.compare("Title:") == 0) && (titleValue.compare(title) == 0))
            {
                copies = std::stoi(copyValue);
                copies++;
                updated = true;
                added = true;
                record = titleKey + " " + titleValue + " " + copyKey + " " + std::to_string(copies);
            }
            else
            {
                record = titleKey + " " + titleValue + " " + copyKey + " " + copyValue;
            }
            tempFile << record << endl;
        }
        if (!added)
        {
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