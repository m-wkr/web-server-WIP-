#include <map>
#include <iostream>
#include <ctime>
#include "response.hpp"

struct response {
  std::string statusCode = "HTTP/1.1 200 OK\r\n";

  std::string rawBody = "<!DOCTYPE html><head></head><body><p>Hi there</p></body>\n";

  std::map<std::string,std::string> headers = {
    {"Content-Type","text/html"},
    {"Content-Length",std::to_string(rawBody.size())},
  };

  void addDateHeader() {
    time_t timestamp = std::time(NULL);
    struct tm datetime = *gmtime(&timestamp);
    char dateString[50];
    std::strftime(dateString,50,"%a, %e %b %Y %H:%M:%S GMT",&datetime);

    headers["Date"] = dateString;
  }

  void concatResponse() {
    constructedMsg += statusCode;
    std::map<std::string,std::string>::iterator i = headers.begin();

    while (i != headers.end()) {
      constructedMsg += i->first + ": " + i->second + "\r\n";
      i++;
    }


    if (headers["Content-Size"] == "0") {
      constructedMsg += "\r\n";
    } else {
      constructedMsg += "\r\n" + rawBody;
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
  currentResponse.addDateHeader();
  currentResponse.concatResponse();
  return currentResponse.getMsg();
}