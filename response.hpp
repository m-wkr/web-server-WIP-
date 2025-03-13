#include <map>
#include <ctime>
#include <iostream>

#include <sstream>
#include <fstream>

#include "mimeTypes.hpp"

struct response {
  std::string statusCodeStr = "HTTP/1.1 ";

  int statusCode = 200;

  std::string rawBody = "";

  std::map<std::string,std::string> headers = {
  };

  void addStatusCode(const int &value) {

    if (value != 200) {
      statusCode = value;
    }

    switch (statusCode) {
      case (200):
        statusCodeStr += "200 OK\r\n";
        break;
      case (400):
        statusCodeStr += "400 Bad Request\r\n";
        break;
      case (403):
      statusCodeStr += "403 Forbidden\r\n";
        break;
      case (404):
        statusCodeStr += "404 Not Found\r\n";
        break;
      case (405):
        statusCodeStr += "405 Method Not Allowed\r\n";
        break;
      case (500):
        statusCodeStr += "500 Internal Server Error\r\n";
        break;
    }
  }

  void addStatusCode(const int &&value) {

    switch (value) {
      case (200):
        statusCodeStr += "200 OK\r\n";
        break;
      case (404):
        statusCodeStr += "404 Not Found\r\n";
        break;
    }
  }

  void addDateHeader() {
    time_t timestamp = std::time(NULL);
    struct tm datetime = *gmtime(&timestamp);
    char dateString[50];
    std::strftime(dateString,50,"%a, %e %b %Y %H:%M:%S GMT",&datetime);

    headers["Date"] = dateString;
  }

  void setGeneralHeaders() {
    headers["Connection"] = "close";
    headers["Server"] = "WIP/1.0.0";
  }

  void setAllowHeader() {
    if (statusCode == 405) {
      headers["Allow"] = "GET, HEAD, PUT, POST";
    }
  }

  void setBody(const std::string &MIMEtype,const std::string &body) {
    rawBody = body;
    headers["Content-Type"] = MIMEtype;
    headers["Content-Length"] = std::to_string(rawBody.size());
  }

  void concatResponse() {
    constructedMsg += statusCodeStr;
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

  void retrieveFile(const std::string &&invokedPath) {
    std::string tempBodyHolder;

    MIME mType = determineMIME(invokedPath);

    if (mType != ERR) {
  
      try {
        std::ifstream targetFile(invokedPath);
        std::string temp;
    
        while (getline(targetFile,temp)) {
          tempBodyHolder += temp;
        }
    
        targetFile.close();
      }
      catch (...) {
        statusCode = 404;
      }

    } else {
      statusCode = 500;

      //Serverside warning
      std::cout << "File retrieval exited. Make sure the file has a valid extension suffix.\n";
    }


    setGeneralHeaders();

    if (statusCode == 200) {
      setBody(getContentType(mType),tempBodyHolder);
    } 
  }

  std::string getMsg() {
    return constructedMsg;
  }

  private:
  std::string constructedMsg;
};