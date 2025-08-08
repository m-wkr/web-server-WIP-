#include "requestParser.hpp"
#include <iostream>

//this needs to be tidied
void parser(request &cRequest) {
  parseState stateTracker = REQLINE_METHOD;

  u_int8_t BUFFERSIZE = 1024;
  char stringBuffer[BUFFERSIZE], secondaryBuffer[BUFFERSIZE];
  int stringBufferPtr = 0, secondaryBufferPtr = 0;

  int i = 0;

  while (i + 1 < BUFFERSIZE) {
    if (cRequest.msgBuffer[i] == '\r' && cRequest.msgBuffer[i+1] == '\n') {

      switch (stateTracker) {
        case REQLINE_HTTP_VER:
          //set versioning (TODO)
          stateTracker = HEADER;
          break;
        case HEADER:
          //error 
          break;
        case HEADER_CONTENT:
          turnHeaderToLowercase(stringBuffer,BUFFERSIZE);
          cRequest.headers[stringBuffer] = secondaryBuffer;
          stateTracker = BEGIN_BODY;
          stringBufferPtr = 0;
          secondaryBufferPtr = 0;
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
        } else if (stateTracker == REQLINE_URI) {
          determineURIType(cRequest,stringBuffer); //towrite
        }
      }

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


  cRequest.rawBody = stringBuffer;
}