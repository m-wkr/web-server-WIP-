#include <string>
#include "requestEnums.hpp"


void turnHeaderToLowercase(char (&header)[],int size);

void strToReqMethod(request &cRequest,const char (&strBuffer)[]);

std::string methodReqToStr(requestTypes &rType);

void checkHost(request &cReq, char (&strBuffer)[], char (&sndBuffer)[]);

void determineURIType(request &cRequest, char (&reqTargetStr)[]);

std::string URITypeToStr(requestURIType &r);

void determineReqURIForm(request &cReq, char (&strBuffer)[]);

void obtainMinor(request &cReq, char (&strBuffer)[]);