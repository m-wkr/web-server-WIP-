#include "response.hpp"


std::string craftResponse(response &currentResponse, std::string &fileContent) {
  currentResponse.setBody(fileContent);
  currentResponse.addDateHeader();
  currentResponse.concatResponse();
  return currentResponse.getMsg();
}