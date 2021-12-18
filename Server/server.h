#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <fstream> 

using std::perror;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::string;

#define PORT 8080
#define CONNECTIONS 1
#define BOOKS "books.txt"
#define USERS "users.txt"
#define TEMP "temp.txt"
#define BUFFER_SIZE 32

void inline Setup();

bool SearchBook(string title);

bool RentBook(string title);