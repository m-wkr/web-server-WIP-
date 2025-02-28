#include <string>
#include <map>
#include <ctime>

struct response {
  std::string statusCode = "HTTP/1.1 ";

  std::string rawBody = "";

  std::map<std::string,std::string> headers = {
  };

  void addStatusCode(int &value) {

    switch (value) {
      case (200):
        statusCode += "200 OK\r\n";
        break;
      case (400):
        statusCode += "400 Bad Request\r\n";
        break;
      case (403):
      statusCode += "403 Forbidden\r\n";
        break;
      case (404):
        statusCode += "404 Not Found\r\n";
        break;
      case (405):
        statusCode += "405 Method Not Allowed\r\n";
        break;
    }
  }

  void addStatusCode(int &&value) {

    switch (value) {
      case (200):
        statusCode += "200 OK\r\n";
        break;
      case (404):
        statusCode += "404 Not Found\r\n";
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

  void setBody(std::string &body) {
    rawBody = body;
    headers["Content-Type"] = "text/html"; //temporary 
    headers["Content-Length"] = std::to_string(rawBody.size());
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