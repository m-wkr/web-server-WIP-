#include "requestParser.hpp"
#include <iostream>


void parser(request &cRequest) {
  parseState stateTracker = REQLINE;
  char stringBuffer[1024], secondaryBuffer[1024];
  int stringBufferPtr = 0, secondaryBufferPtr = 0;

  int i = 0;

  while (i + 1 < 1024) {
    if (cRequest.msgBuffer[i] == '\r' && cRequest.msgBuffer[i+1] == '\n') {

      switch (stateTracker) {
        case REQLINE:
          stateTracker = HEADER;
          break;
        case HEADER:
          //error 
          break;
        case HEADERCONTENT:
          cRequest.headers[stringBuffer] = secondaryBuffer;
          stateTracker = BEGINBODY;
          stringBufferPtr = 0;
          secondaryBufferPtr = 0;
          break;
        case BEGINBODY:
          stateTracker = BODY;
          break;
        case BODY:
          break;
      }

      i+=2;

    } else {

      if (stateTracker == BEGINBODY) {
        stateTracker = HEADER;
      }

      if (stateTracker == HEADER && cRequest.msgBuffer[i] == ':') {
        stateTracker = HEADERCONTENT;
        i++;
        continue;
      }

      if (stateTracker == HEADERCONTENT) {
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