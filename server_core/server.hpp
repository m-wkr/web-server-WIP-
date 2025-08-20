#include "socketWrapper.hpp"
#include "response.hpp"
#include <iostream>
#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
//#include <thread>

//std::atomic_bool listening = true;

namespace helpers {
  void trace(request &req, response &res) {
    res.setBody(MESSAGE_HTTP,req.msgBuffer);
  }

  void setListeningState() {
    std::string test;
    while (!(std::cin >> test)) {
    }
    //listening = false;
  };
}

void setNonblocking(int fd) {
  int flags = fcntl(fd,F_GETFL,0);

  flags |= O_NONBLOCK;
  fcntl(fd,F_SETFL,flags);
}


class server {
  static std::string hostName;
  static std::map<std::string,bool> hostAliases;

  //function ptr file handlers
  typedef void (*resourceFunction)(request &req, response &res);
  static std::unordered_map<std::string,std::unordered_map<requestTypes,resourceFunction>> pathHandler;

  //NOTE: headers are all lowercase, so header access must be paid in mind to this fact

  //Socket holder
  socketWrapper serverSocket;

  class clientHandler {
    request currentRequest;
    response responseToBeSent;

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
        responseToBeSent.reqType = currentRequest.method; //NEW THING
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
      std::unordered_map<requestTypes,resourceFunction> &currentResource = pathHandler[currentRequest.requestTarget];
  
  
      if (currentRequest.URIType == GENERAL) {
        std::unordered_map<requestTypes,resourceFunction> &currentResource = pathHandler["*"];
      } 
  
      std::string availableMethods = "";
  
      int counter = 0;
      for (std::unordered_map<requestTypes,resourceFunction>::iterator iter = currentResource.begin(); iter != currentResource.end(); iter++) {
        availableMethods += requestTypeToString(iter->first);
  
        if (counter + 1 < currentResource.size()) {
          availableMethods += ", ";
        }
  
        counter++;
      }
  
      return availableMethods;
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
    public:
    void handleClient(int clientSocketFD) { //change to fd!
      //int clientSocketFD = accept(serverSocket.getFD(),nullptr,nullptr);

      //Obtain request info
      recv(clientSocketFD,currentRequest.msgBuffer,sizeof(currentRequest.msgBuffer),0);
      parser(currentRequest);
  
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
    }
  };

  public:
  server(std::string &&nHostName) {
    hostName = nHostName;
    hostAliases.insert({nHostName,true});
  }

  std::string getHostName() {
    return hostName;
  }

  //handles GET & HEAD
  void get(const std::string &path, void (*fPtr)(request &req,response &res)) {
    pathHandler[path][GET] = fPtr;
    pathHandler["*"][GET] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;  
  }

  void post(const std::string &path, void (*fPtr)(request &req, response &res)) {
    pathHandler[path][POST] = fPtr;
    pathHandler["*"][POST] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;
  }

  void put(const std::string &path, void (*fPtr)(request &req, response &res)) {
    pathHandler[path][PUT] = fPtr;
    pathHandler["*"][PUT] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;
  }

  void deleteSource(const std::string &path, void (*fPtr)(request &req, response &res)) {
    pathHandler[path][DELETE] = fPtr;
    pathHandler["*"][DELETE] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;
  }

  void enableTrace(const std::string &path) {
    pathHandler[path][TRACE] = helpers::trace;
    pathHandler["*"][TRACE] = nullptr;
    pathHandler[path][OPTIONS] = nullptr;
  }

  void startListening() {
    listen(serverSocket.getFD(),5);

    struct epoll_event event, eventsQueue[1024];
    int epollFD = epoll_create1(0);

    event.events = EPOLLIN;
    event.data.fd = serverSocket.getFD();

    setNonblocking(serverSocket.getFD());


    epoll_ctl(epollFD, EPOLL_CTL_ADD, serverSocket.getFD(), &event);


    while (true) {
      int numEvents = epoll_wait(epollFD,eventsQueue,5,-1);

      for (int i = 0; i < numEvents; i++) {
        //peer connection
        if (eventsQueue[i].data.fd == serverSocket.getFD()) {
          int newClientFD = accept(serverSocket.getFD(),nullptr,nullptr);
          setNonblocking(newClientFD);
          struct epoll_event ev;
          ev.events = EPOLLIN; //Notify for read events
          ev.events |= EPOLLOUT; //Notify for write events
          ev.data.fd = newClientFD;

          epoll_ctl(epollFD,EPOLL_CTL_ADD, newClientFD, &ev);
        } else {
          //peer socket ready

          if (eventsQueue[i].events & (EPOLLIN | EPOLLOUT)) {
            int curClientFD = eventsQueue[i].data.fd;
            clientHandler cClient;
            cClient.handleClient(curClientFD);

            epoll_ctl(epollFD, EPOLL_CTL_DEL, curClientFD,NULL);
            close(curClientFD);
          } 
        }
      }
    }

    close(epollFD);    
    close(serverSocket.getFD());
  }
};

std::map<std::string,bool> server::hostAliases = {{"localhost",true},{"127.0.0.1",true}};
std::string server::hostName = "";
typedef void (*resourceFunction)(request &req, response &res);
std::unordered_map<std::string,std::unordered_map<requestTypes,resourceFunction>> server::pathHandler = {};