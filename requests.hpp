#include <map>
#include <string>

//WIP TO BE EXPANDED
enum requestTypes {
  GET,
  HEAD,
  PUT,
  POST
};

enum requestURIType {
  GENERAL,
  ABS_URI,
  ABS_PATH,
  AUTHORITY
};
  
struct request {
  requestTypes method = HEAD;
  requestURIType URIType = ABS_PATH; //default assumption
  std::string requestTarget;
  int minorVersion = 1;
  std::map<std::string,std::string> headers;
  
  int errorCode = 200;
  
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