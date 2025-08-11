#include <string>
#include <algorithm>
#include <string.h>
#include "requestHelper.hpp"

void turnHeaderToLowercase(char (&header)[],u_int8_t size) {

  for (int i = 0; i < size; i++) {
    if (header[i] == '\0') break;

    header[i] = std::tolower(header[i]);
  }
}

void strToReqMethod(request &cRequest,const char (&strBuffer)[]) {
  if (strcmp(strBuffer,"OPTIONS") == 0) {
    cRequest.method = OPTIONS;
  } else if (strcmp(strBuffer,"GET") == 0) {
    cRequest.method = GET;
  } else if (strcmp(strBuffer,"HEAD") == 0) {
    cRequest.method = HEAD;
  } else if (strcmp(strBuffer,"POST") == 0) {
    cRequest.method = POST;
  } else if (strcmp(strBuffer,"PUT") == 0) {
    cRequest.method = PUT;
  } else if (strcmp(strBuffer,"DELETE") == 0) {
    cRequest.method = DELETE;
  } else if (strcmp(strBuffer,"TRACE") == 0) {
    cRequest.method = TRACE;
  } else {
    cRequest.errorCode = 501;
  }
}

std::string methodReqToStr(requestTypes &rType) {
  switch (rType) {
    case OPTIONS:
      return "OPTIONS";
    case GET:
      return "GET";
    case HEAD:
      return "HEAD";
    case POST:
      return "POST";
    case PUT:
      return "PUT";
    case DELETE:
      return "DELETE";
    case TRACE:
      return "TRACE";
  }
}

void determineURIType(request &cRequest, char (&reqTargetStr)[]) {
  if (reqTargetStr == "*") {
    cRequest.URIType = GENERAL;
  } else if (reqTargetStr == "http://") {
    cRequest.URIType = ABS_URI;
  } else if (reqTargetStr == "authority") {
    cRequest.URIType = AUTHORITY;
  } else {
    cRequest.URIType =  ABS_PATH;
  }
}