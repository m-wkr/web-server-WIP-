#include "requests.hpp"
#include "socketWrapper.hpp"
#include "response.hpp"
#include <iostream>



class server {
  std::string hostName;
  std::map<std::string,bool> hostAliases = {{"localhost",true},{"127.0.0.1",true}};
  //function ptr file handlers
  //std::map<std::string,void (*)(request &req, response &res)> pathHandler = {};

  typedef void (*resourceFunction)(request &req, response &res);
  std::map<std::string,std::map<requestTypes,resourceFunction>> pathHandler = {};

  //Request holder - NOTE: headers are all lowercase, so header access must be paid in mind to this fact
  request currentRequest;
  //Response holder
  response responseToBeSent;
  //Socket holder
  socketWrapper serverSocket;

  void checkIfMethodIsAllowed(requestTypes &currentReqType) {
    if (pathHandler[currentRequest.requestTarget].count(currentReqType)) {
      pathHandler[currentRequest.requestTarget][currentReqType](currentRequest,responseToBeSent);
    } else {
      currentRequest.errorCode = 405;
    }
  }

  void delegateResourceByPath() {
    // Check resource is registered
    if (pathHandler.count(currentRequest.requestTarget)) {

      requestTypes simplifiedReqType = GET;

      switch (currentRequest.method) {
        case GET:
        case HEAD:
          break;
        case PUT:
          simplifiedReqType = PUT;
          break;
        case POST:
          simplifiedReqType = POST;
          break;
        case OPTIONS:
          simplifiedReqType = OPTIONS;
          break;
      }

      checkIfMethodIsAllowed(simplifiedReqType);

    } else {
      currentRequest.errorCode = 404;
    }
  }


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

  std::string requestTypeToString(const requestTypes &rType) {
    switch (rType) {
      case OPTIONS:
        return "OPTIONS";
      case GET:
      case HEAD:
        return "GET, HEAD";
      case PUT:
        return "PUT";
      case POST:
        return "POST";
    }
  }

  std::string concatMethods() {
    std::map<requestTypes,resourceFunction> &currentResource = pathHandler[currentRequest.requestTarget];

    std::string availableMethods = "";

    for (std::map<requestTypes,resourceFunction>::iterator iter = currentResource.begin(); iter != currentResource.end(); iter++) {
      availableMethods += requestTypeToString(iter->first);

      if (iter->first != currentResource.rbegin()->first) {
        availableMethods += ", ";
      }
    }

    return availableMethods;
  }

  //renamed to get from manageConnection - handles GET & HEAD
  void get(const std::string &path, void (*fPtr)(request &req,response &res)) {
    pathHandler[path][GET] = fPtr;
  }

  void options(const std::string &path, void (*fPtr)(request &req,response &res)) {
    pathHandler[path][OPTIONS] = fPtr;    
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
    delegateResourceByPath();


    responseToBeSent.addStatusCode(currentRequest.errorCode);


    //2. Construct resource & send it
    responseToBeSent.addDateHeader();
    responseToBeSent.setGeneralHeaders();
    if (responseToBeSent.statusCode == 405 || currentRequest.method == OPTIONS) {
      responseToBeSent.setAllowHeader(concatMethods());
    }
    responseToBeSent.concatResponse();
    std::string responseMsg = responseToBeSent.getMsg();

    send(clientSocketFD,responseMsg.c_str(),responseMsg.size(),0);
    close(serverSocket.getFD());
  }
};