#include <map>
#include <string>

enum requestTypes : u_int8_t {
  OPTIONS,
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  TRACE
};

enum requestURIType : u_int8_t {
  GENERAL,
  ABS_URI,
  ABS_PATH,
  AUTHORITY
};
  
struct request {
  char msgBuffer[2048] = "GET / HTTP/1.1\r\nHost: local host\r\nUser-Agent: curl/8.11.1\r\nAccept: */*\r\n\r\nhi\r\n";
  requestTypes method = HEAD;
  requestURIType URIType = ABS_PATH; //default assumption
  std::string requestTarget;
  int minorVersion = 1;
  std::map<std::string,std::string> headers;
  int errorCode = 200;
  std::string rawBody;

  std::string getSpecifiedHeader(const std::string &headerName) {
    if (headers.contains(headerName)) return headers[headerName];
    else return "";
  }
};