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

enum resourceType {
  STATIC,
  PROGRAM_SET
};


struct designatedResource{
  resourceType type;
  std::string contentHolder; //either file path or actual payload (rename to payload soon)
};


class server {
  //Hold file path abstraction mappings
  std::map<std::string, designatedResource> resourceHandlers = {};

  //Request holder
  request currentRequest;
  //Response holder
  response responseToBeSent;
  //Socket holder
  socketWrapper serverSocket;


  public:
  //Adds a default path - not allowing much (if any) tampering and currently only allows service of just one file
  void addFileHandler(const std::string& path, const std::string& filePath) {
    resourceHandlers[path] = designatedResource({STATIC, filePath});
  }

  resourceStatus retrieveResource(std::string& invokedPath) {
    resourceStatus currentResource;

    try {
      std::ifstream targetFile(resourceHandlers.at(invokedPath).contentHolder);
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



  void manageConnection(const std::string &path, void (*fPtr)(request &req,response &res)) {
    (*fPtr)(currentRequest,responseToBeSent);
    resourceHandlers[path] = designatedResource({PROGRAM_SET, ""});

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

    if (resourceHandlers[currentRequest.requestTarget].type == STATIC) {
      resourceStatus resource = retrieveResource(currentRequest.requestTarget);
      responseToBeSent.setBody(resource.body);
    } //else expect body to be set by users
    //2. Construct resource & send it
    responseToBeSent.addDateHeader();
    responseToBeSent.concatResponse();
    responseMsg = responseToBeSent.getMsg();

    send(clientSocketFD,responseMsg.c_str(),responseMsg.size(),0);
    close(serverSocket.getFD());
  }
};