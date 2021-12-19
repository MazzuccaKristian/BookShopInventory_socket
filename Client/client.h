#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <unistd.h>

#define PORT 8989
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 2048

using std::perror;
using std::cout;
using std::endl;
using std::cin;
using std::string;

void ShowMainMenu();

int GetUserChoice();

string inline GetData(bool dumpFlag);

int ServerConnection();