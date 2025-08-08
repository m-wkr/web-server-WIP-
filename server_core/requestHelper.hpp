#include <string>
#include "requestEnums.hpp"


void turnHeaderToLowercase(char (&header)[],u_int8_t size);

void strToReqMethod(request &cRequest,const char (&strBuffer)[]);

void determineURIType(request &cRequest, char (&reqTargetStr)[]);

void determineURIType(request &cRequest, char (&reqTargetStr)[]);