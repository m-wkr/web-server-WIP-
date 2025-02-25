#include "requests.hpp"
#include "socketWrapper.hpp"
#include "response.hpp"
#include <iostream>
#include <sstream>
#include <fstream>



struct resourceStatus {
  int statusCode = 200;
  std::string body;
};



class server {
  //Hold file path abstraction mappings

  //Request holder
  request currentRequest;
  //Response holder
  response responseToBeSent;
  //Socket holder
  socketWrapper serverSocket;


  public:
  void manageConnection(const std::string &path, void (*fPtr)(request &req,response &res)) {
    (*fPtr)(currentRequest,responseToBeSent);
    //resourceHandlers[path] = designatedResource({USER_SET, ""});

  }




  void startListening() {
    listen(serverSocket.getFD(),5);

    int clientSocketFD = accept(serverSocket.getFD(),nullptr,nullptr);

    char buffer[2048];

    //Obtain request info
    recv(clientSocketFD,buffer,sizeof(buffer),0);
    requestParser(buffer,currentRequest);
    //Create response
    //1. Fetch requested resource
    std::string responseMsg;    
    
    //else expect body to be set by users
    //2. Construct resource & send it
    responseToBeSent.addDateHeader();
    responseToBeSent.concatResponse();
    responseMsg = responseToBeSent.getMsg();

    send(clientSocketFD,responseMsg.c_str(),responseMsg.size(),0);
    close(serverSocket.getFD());
  }
};


resourceStatus retrieveFile(std::string &&invokedPath) {
  resourceStatus currentResource;

  try {
    std::ifstream targetFile(invokedPath);
    std::string temp;

    while (getline(targetFile,temp)) {
      currentResource.body += temp;
    }

    targetFile.close();
  }
  catch (...) {
    currentResource.statusCode = 404;
  }

  return currentResource;
}