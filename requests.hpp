#include <map>
#include <string>

//WIP TO BE EXPANDED
enum requestTypes {
  GET,
  HEAD,
  PUT,
  POST
};
  
struct request {
  requestTypes method = HEAD;
  std::string requestTarget;
  std::map<std::string,std::string> headers;
  
  int errorCode = 0;
  
  std::string getBody() {
    return rawBody;
  }

  std::string getSpecifiedHeader(const std::string &headerName) {
    try {
      return headers.at(headerName);
    }
    catch (...) {
      return "";
    }
    
  }
  
  private:
  std::string rawBody;
};


void requestParser(const char* &&buffer, request &currentRequest);