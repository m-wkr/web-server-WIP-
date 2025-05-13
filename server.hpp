#include "requests.hpp"
#include "socketWrapper.hpp"
#include "response.hpp"
#include <iostream>

namespace helpers {
  void trace(request &req, response &res) {
    res.setBody(MESSAGE_HTTP,req.msgBuffer);
  }
}

class server {
  std::string hostName;
  std::map<std::string,bool> hostAliases = {{"localhost",true},{"127.0.0.1",true}};

  //function ptr file handlers
  typedef void (*resourceFunction)(request &req, response &res);
  std::map<std::string,std::map<requestTypes,resourceFunction>> pathHandler = {};

  //Request holder - NOTE: headers are all lowercase, so header access must be paid in mind to this fact
  request currentRequest;
  //Response holder
  response responseToBeSent;
  //Socket holder
  socketWrapper serverSocket;


  void handleOptions() {
    if (currentRequest.URIType == GENERAL && currentRequest.requestTarget == "*"){
      return;
    } else if (!pathHandler[currentRequest.requestTarget].count(OPTIONS)) {
      currentRequest.errorCode = 405;
    }
  }

  void genericMethodHandler(const requestTypes &&currentReqType) {
    if (pathHandler[currentRequest.requestTarget].count(currentReqType)) {
      pathHandler[currentRequest.requestTarget][currentReqType](currentRequest,responseToBeSent);
    } else {
      currentRequest.errorCode = 405;
    }
  }

  void delegateResourceByPath() {
    // Check resource is registered
    if (pathHandler.count(currentRequest.requestTarget)) {

      switch (currentRequest.method) {
        case OPTIONS:
          handleOptions();
          break;
        case GET:
        case HEAD:
          genericMethodHandler(GET);
          break;
        case POST:
          genericMethodHandler(POST);
          break;
        case PUT:
          genericMethodHandler(PUT);
          break;
        case DELETE:
          genericMethodHandler(DELETE);
          break;
        case TRACE:
          genericMethodHandler(TRACE); 
          break;
      }

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

  bool validateHost() {
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
      case POST:
        return "POST";
      case PUT:
        return "PUT";
      case DELETE:
        return "DELETE";
      case TRACE:
        return "TRACE";
    }
  }

  std::string concatMethods() {
    std::map<requestTypes,resourceFunction> &currentResource = pathHandler[currentRequest.requestTarget];


    if (currentRequest.URIType == GENERAL) {
      std::map<requestTypes,resourceFunction> &currentResource = pathHandler["*"];
    } 

    std::string availableMethods = "";

    for (std::map<requestTypes,resourceFunction>::iterator iter = currentResource.begin(); iter != currentResource.end(); iter++) {
      availableMethods += requestTypeToString(iter->first);

      if (iter->first != currentResource.rbegin()->first) {
        availableMethods += ", ";
      }
    }

    return availableMethods;
  }

  //handles GET & HEAD
  void get(const std::string &path, void (*fPtr)(request &req,response &res)) {
    pathHandler[path][GET] = fPtr;
    pathHandler["*"][GET] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;  
  }

  void post(const std::string &path, void (*fPtr)(request &req, response &res), int statusCode = 200) {
    pathHandler[path][POST] = fPtr;
    pathHandler["*"][POST] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;
    currentRequest.errorCode = statusCode;
  }

  void put(const std::string &path, void (*fPtr)(request &req, response &res), int statusCode = 201) {
    pathHandler[path][PUT] = fPtr;
    pathHandler["*"][PUT] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;
    currentRequest.errorCode = statusCode;
  }

  void deleteSource(const std::string &path, void (*fPtr)(request &req, response &res), int statusCode) {
    pathHandler[path][DELETE] = fPtr;
    pathHandler["*"][DELETE] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;
    currentRequest.errorCode = statusCode;
  }

  void enableTrace(const std::string &path) {
    pathHandler[path][TRACE] = helpers::trace;
    pathHandler["*"][TRACE] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;
  }

  void startListening() {
    listen(serverSocket.getFD(),5);

    int clientSocketFD = accept(serverSocket.getFD(),nullptr,nullptr);

    //Obtain request info
    recv(clientSocketFD,currentRequest.msgBuffer,sizeof(currentRequest.msgBuffer),0);
    requestParser(currentRequest);

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