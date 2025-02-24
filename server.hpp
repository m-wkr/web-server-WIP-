#include "requests.hpp"
#include "socketWrapper.hpp"
#include "response.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

//Serving static content

struct resourceStatus {
  int statusCode = 200;
  std::string body;
};

class server {
  //Hold file path abstraction mappings
  std::map<std::string,std::string> fileHandlers = {};

  //Request holder
  request currentRequest;
  //Response holder
  response responseToBeSent;
  //Socket holder
  socketWrapper serverSocket;


  public:
  //Adds a default path - not allowing much (if any) tampering and currently only allows service of just one file
  void addFileHandler(const std::string& path, const std::string& filePath) {
    fileHandlers[path] = filePath;
  }

  resourceStatus retrieveResource(std::string& invokedPath) {
    resourceStatus currentResource;

    try {
      std::ifstream targetFile(fileHandlers.at(invokedPath));
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








  void startListening() {
    listen(serverSocket.getFD(),5);

    int clientSocketFD = accept(serverSocket.getFD(),nullptr,nullptr);

    char buffer[2048];

    //Obtain request info
    recv(clientSocketFD,buffer,sizeof(buffer),0);
    requestParser(buffer,currentRequest);
    //Create response
    //1. Fetch requested resource
    resourceStatus resource = retrieveResource(currentRequest.requestTarget);



    //2. Construct resource & send it
    std::string response = craftResponse(responseToBeSent,resource.body);
    send(clientSocketFD,response.c_str(),response.size(),0);
    close(serverSocket.getFD());
  }
};