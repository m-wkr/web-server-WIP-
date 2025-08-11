#include <string>
#include "requestEnums.hpp"


void turnHeaderToLowercase(char (&header)[],u_int8_t size);

void strToReqMethod(request &cRequest,const char (&strBuffer)[]);

std::string methodReqToStr(requestTypes &rType);

void determineURIType(request &cRequest, char (&reqTargetStr)[]);