#include "requestParser.hpp"


//move to a reqHelper file
void turnHeaderToLowercase(std::string &header);

requestURIType determineRequestURIForm(std::string &requestTarget);

void separateResourceFromHostName(std::string &ABSURI, request &request);

void parseStartLine(std::string &startLine, request &request);

std::string ignoreLWS(const std::string &temp);

//implement the host part in requestParser or in a separate file
void spliceHeaders(std::string &headerLine, request &request);



void requestParser(request &currentRequest);