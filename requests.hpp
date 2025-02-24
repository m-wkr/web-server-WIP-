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
  
  //Create handler for rawBody
  
  private:
  std::string rawBody;
};


void requestParser(const char* &&buffer, request &currentRequest);