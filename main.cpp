
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>




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
    send(clientSocketFD,"HTTP/1.1 403 forbidden\nContent-Type: text/html\n\n<!DOCTYPE html><head></head><body><p>Hi\0there</p></body>\n",sizeof("HTTP/1.1 403 forbidden\nContent-Type: text/html\n\n<!DOCTYPE html><head></head><body><p>Hi</p></body>\n"),0);
    close(serverSocket.getFD());

    return 0;
}