#include "requests.hpp"
#include "socketWrapper.hpp"
#include "response.hpp"
#include <iostream>



class server {
  std::string hostName;
  std::map<std::string,bool> hostAliases = {{"localhost",true},{"127.0.0.1",true}};
  //function ptr file handlers
  std::map<std::string,void (*)(request &req, response &res)> pathHandler = {};

  //Request holder - NOTE: headers are all lowercase, so header access must be paid in mind to this fact
  request currentRequest;
  //Response holder
  response responseToBeSent;
  //Socket holder
  socketWrapper serverSocket;


  public:
  server(std::string &&nHostName) {
    hostName = nHostName;
    hostAliases.insert({nHostName,true});
  }

  std::string getHostName() {
    return hostName;
  }

  //not working
  bool validateHost() {
    //
    if (currentRequest.URIType == ABS_URI) {

      if (currentRequest.requestTarget.length() - 7 < hostName.length()) {
        return false;
      } 

      return hostAliases.count(currentRequest.headers["host"]) != 0;

    }

    //Check if host name matches 
    return hostAliases.count(currentRequest.headers["host"]) != 0;
  }

  void manageConnection(const std::string &path, void (*fPtr)(request &req,response &res)) {
    pathHandler[path] = fPtr;
  }

  void startListening() {
    listen(serverSocket.getFD(),5);

    int clientSocketFD = accept(serverSocket.getFD(),nullptr,nullptr);

    char buffer[2048];

    //Obtain request info
    recv(clientSocketFD,buffer,sizeof(buffer),0);
    requestParser(buffer,currentRequest);

    if (!validateHost()) {
      currentRequest.errorCode = 400;
    }
  
    //expect body to be set by users
    try {
      if (currentRequest.errorCode == 200) {
        pathHandler.at(currentRequest.requestTarget)(currentRequest,responseToBeSent);
      }
    }
    catch (...) {
      currentRequest.errorCode = 404;
    }

    responseToBeSent.addStatusCode(currentRequest.errorCode);


    //2. Construct resource & send it
    responseToBeSent.addDateHeader();
    responseToBeSent.setGeneralHeaders();
    responseToBeSent.setAllowHeader();
    responseToBeSent.concatResponse();
    std::string responseMsg = responseToBeSent.getMsg();

    send(clientSocketFD,responseMsg.c_str(),responseMsg.size(),0);
    close(serverSocket.getFD());
  }
};