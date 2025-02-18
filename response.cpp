#include <map>
#include <iostream>
#include "response.hpp"

struct response {
  std::string statusCode = "HTTP/1.1 200 OK\r\n";
  std::map<std::string,std::string> headers = {
    {"Content-Type","text/html"},
    {"Content-Length",std::to_string(sizeof("<!DOCTYPE html><head></head><body><p>Hi there</p></body>\n"))},
    //{"Date","Tue, 18 Feb 2025 16:56:32 GMT"}
  };
  std::string rawBody = "<!DOCTYPE html><head></head><body><p>Hi there</p></body>\n";

  void concatResponse() {
    constructedMsg += statusCode;
    std::map<std::string,std::string>::iterator i = headers.begin();

    while (i != headers.end()) {
      constructedMsg += i->first + ": " + i->second + '\n';
      i++;
    }


    if (headers["Content-Size"] == "0") {
      constructedMsg += '\n';
    } else {
      constructedMsg += "\n" + rawBody;
    }

  }

  std::string getMsg() {
    return constructedMsg;
  }

  private:
  std::string constructedMsg;
};



std::string craftResponse() {
  response currentResponse;
  currentResponse.concatResponse();
  return currentResponse.getMsg();
}