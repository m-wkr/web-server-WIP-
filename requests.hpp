#include <map>
#include <string>

enum requestTypes : uint8_t {
  OPTIONS,
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  TRACE
};

enum requestURIType : uint8_t {
  GENERAL,
  ABS_URI,
  ABS_PATH,
  AUTHORITY
};
  
struct request {
  char msgBuffer[2048];
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


void requestParser(request &currentRequest);