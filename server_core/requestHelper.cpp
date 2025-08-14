#include <string>
#include <algorithm>
#include <string.h>
#include <iostream>
#include "requestHelper.hpp"

void turnHeaderToLowercase(char (&header)[],int size) {

  for (int i = 0; i < size; i++) {
    if (header[i] == '\0') break;

    header[i] = std::tolower(header[i]);
  }
}

void checkHost(request &cReq, char (&strBuffer)[], char (&sndBuffer)[]) {
  if (cReq.headers.contains("host") && strcmp(strBuffer,"host") == 0 && cReq.URIType != ABS_URI) {
    cReq.errorCode = 400;
  } else if (cReq.URIType != ABS_URI && strcmp(strBuffer,"host") == 0){
    cReq.headers[strBuffer] = sndBuffer;
  } 

  
  if (strcmp(strBuffer,"host")) {
    cReq.headers[strBuffer] = sndBuffer;
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

std::string URITypeToStr(requestURIType &r) {
  switch (r) {
    case GENERAL:
      return "GENERAL";
    case ABS_URI:
      return "ABS_URI";
    case AUTHORITY:
      return "AUTHORITY";
    default:
      return "ABS_PATH";
  }
}

void determineReqURIForm(request &cReq, char (&strBuffer)[]) {

  if (strcmp(strBuffer,"*") == 0) {
    cReq.URIType = GENERAL;
  } else if (strncmp(strBuffer,"http://",7) == 0) {
    cReq.URIType = ABS_URI;
  } else if (strcmp(strBuffer,"authority") == 0) {
    cReq.URIType = AUTHORITY;
  } else {
    cReq.URIType = ABS_PATH;
  }

  if (cReq.URIType == ABS_URI) {
    char temp[1024];

    for (int i = 7; i < 1024; i++) {
      if (strBuffer[i] == '/') {
        strncpy(temp,strBuffer+7,i-7);
        cReq.headers["host"] = temp;
        strcpy(temp,strBuffer+i);
        cReq.requestTarget = temp;
        return;
      }
    }

    cReq.errorCode = 400;

    

  
  } else {
    cReq.requestTarget = strBuffer;
  }
}

void obtainMinor(request &cReq, char (&strBuffer)[]) {
  if (strcmp(strBuffer,"HTTP/1.0") == 0) {
    cReq.minorVersion = 0;
  } else if (strcmp(strBuffer,"HTTP/1.1") != 0) {
    cReq.errorCode = 403;
  }
}

void requestDebug(request &cRequest) {
  std::cout << methodReqToStr(cRequest.method) << "-" << cRequest.requestTarget << "-" << URITypeToStr(cRequest.URIType) << "-" << cRequest.minorVersion << cRequest.errorCode << '\n';

  std::map<std::string,std::string>::iterator it = cRequest.headers.begin();
  while (it != cRequest.headers.end()) {
    std::cout << it->first << "__" << it->second << '\n';
    it++;
  }

  std::cout << cRequest.rawBody << '\n';
}
