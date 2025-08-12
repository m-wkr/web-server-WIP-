#include "requestParser.hpp"
#include <iostream>

void resetPtr(int &ptr,char (&ptrB)[]) {
  ptrB[ptr] = '\0';
  ptr = 0;
}

//this needs to be tidied
void parser(request &cRequest) {
  parseState stateTracker = REQLINE_METHOD;

  int BUFFERSIZE = 1024;
  char stringBuffer[BUFFERSIZE], secondaryBuffer[BUFFERSIZE];
  int stringBufferPtr = 0, secondaryBufferPtr = 0;

  int i = 0;

  while (i + 1 < BUFFERSIZE) {
    if (cRequest.msgBuffer[i] == '\r' && cRequest.msgBuffer[i+1] == '\n') {

      switch (stateTracker) {
        case REQLINE_HTTP_VER:
          stateTracker = HEADER;
          resetPtr(stringBufferPtr,stringBuffer);
          obtainMinor(cRequest,stringBuffer);
          break;
        case HEADER:
          //error 
          break;
        case HEADER_CONTENT:
          resetPtr(stringBufferPtr,stringBuffer);
          resetPtr(secondaryBufferPtr,secondaryBuffer);

          turnHeaderToLowercase(stringBuffer,BUFFERSIZE);
          checkHost(cRequest,stringBuffer,secondaryBuffer);
          stateTracker = BEGIN_BODY;

          break;
        case BEGIN_BODY:
          stateTracker = BODY;
          break;
        case BODY:
          break;
      }

      i+=2;

    } else {

      if (stateTracker == BEGIN_BODY) {
        stateTracker = HEADER;
      }

      if (stateTracker == HEADER && cRequest.msgBuffer[i] == ':') {
        stateTracker = HEADER_CONTENT;
        i++;
        continue;
      }

      if (cRequest.msgBuffer[i] == ' ') {
        if (stateTracker == REQLINE_METHOD) {
          strToReqMethod(cRequest,stringBuffer); //towrite
          stateTracker = REQLINE_URI;
          resetPtr(stringBufferPtr,stringBuffer);

          
        } else if (stateTracker == REQLINE_URI) {
          determineURIType(cRequest,stringBuffer); //towrite
          stateTracker = REQLINE_HTTP_VER;
          resetPtr(stringBufferPtr,stringBuffer);
          determineReqURIForm(cRequest,stringBuffer);
        }
      } else 

      if (stateTracker == HEADER_CONTENT) {
        secondaryBuffer[secondaryBufferPtr] = cRequest.msgBuffer[i];
        secondaryBufferPtr++;
      } else {
        stringBuffer[stringBufferPtr] = cRequest.msgBuffer[i];
        stringBufferPtr++;
      }

      i++;
    }
  }

  resetPtr(stringBufferPtr,stringBuffer);
  cRequest.rawBody = stringBuffer;

  /*std::cout << methodReqToStr(cRequest.method) << "-" << cRequest.requestTarget << "-" << URITypeToStr(cRequest.URIType) << "-" << cRequest.minorVersion << cRequest.errorCode << '\n';

  std::map<std::string,std::string>::iterator it = cRequest.headers.begin();
  while (it != cRequest.headers.end()) {
    std::cout << it->first << "__" << it->second << '\n';
    it++;
  }

  std::cout << cRequest.rawBody;*/
}