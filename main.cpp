
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "requests.hpp"
#include "response.hpp"

#include <iostream>


class socketWrapper {
  int socketFD = socket(AF_INET,SOCK_STREAM,0);
  sockaddr_in address;

  public:
  socketWrapper() {
    address.sin_family = AF_INET;
    address.sin_port = htons(80);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(socketFD,(struct sockaddr*)&address,sizeof(address));
  }

  int getFD() {
    return socketFD;
  }
};

socketWrapper serverSocket;

int main() {
    listen(serverSocket.getFD(),5);

    int clientSocketFD = accept(serverSocket.getFD(),nullptr,nullptr);

    char buffer[2048];

    recv(clientSocketFD,buffer, sizeof(buffer),0);
    requestParser(buffer);
    std::string response = craftResponse();
    send(clientSocketFD,response.c_str(),response.size(),0);
    close(serverSocket.getFD());

    return 0;
}