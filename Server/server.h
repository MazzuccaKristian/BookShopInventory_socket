#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>

using std::perror;
using std::cout;
using std::endl;

#define PORT 8080
#define CONNECTIONS 1