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