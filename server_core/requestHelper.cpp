#include <string>
#include <algorithm>
#include "requestHelper.hpp"

void turnHeaderToLowercase(char (&header)[],u_int8_t size) {

  for (int i = 0; i < size; i++) {
    if (header[i] == '\0') break;

    header[i] = std::tolower(header[i]);
  }
}

void strToReqMethod(request &cRequest,const char (&strBuffer)[]) {
  if (strBuffer == "OPTIONS") {
    cRequest.method = OPTIONS;
  } else if (strBuffer == "GET") {
    cRequest.method = GET;
  } else if (strBuffer == "HEAD") {
    cRequest.method = HEAD;
  } else if (strBuffer == "POST") {
    cRequest.method = POST;
  } else if (strBuffer == "PUT") {
    cRequest.method = PUT;
  } else if (strBuffer == "DELETE") {
    cRequest.method = DELETE;
  } else if (strBuffer == "TRACE") {
    cRequest.method = TRACE;
  } else {
    cRequest.errorCode = 501;
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