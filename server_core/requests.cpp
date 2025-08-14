#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "requests.hpp" 

//move to a reqHelper file
void turnHeaderToLowercase(std::string &header) {
  std::transform(header.begin(),header.end(),header.begin(),[](unsigned char c) { return std::tolower(c); });
}

requestURIType determineRequestURIForm(std::string &requestTarget) {

  if (requestTarget == "*") {
    return GENERAL;
  } else if (requestTarget.length() > 7 && requestTarget.substr(0,7) == "http://") {
    return ABS_URI;
  } else if (requestTarget == "authority") {
    return AUTHORITY;
  } else {
    return ABS_PATH;
  }
}

void separateResourceFromHostName(std::string &ABSURI, request &request) {
  ABSURI = ABSURI.substr(7);

  for (int i = 0; i < ABSURI.length(); i++) {
    if (ABSURI[i] == '/') {
      request.headers["host"] = ABSURI.substr(0,i);
      request.requestTarget = ABSURI.substr(i);
      return;
    }
  }

  request.errorCode = 400;
}

void parseStartLine(std::string &startLine, request &request) {
  std::stringstream rawString(startLine);
  std::string temp;

  int i = 0;

  while (getline(rawString,temp,' ')) {
    if (i == 0) {
      if (temp == "OPTIONS") {
        request.method = OPTIONS;
      } else if (temp == "GET") {
        request.method = GET;
      } else if (temp == "HEAD") {
        request.method = HEAD;
      } else if (temp == "PUT") {
        request.method = PUT;
      } else if (temp == "POST") {
        request.method = POST;
      } else if (temp == "TRACE") {
        request.method = TRACE;
      } else {
        request.errorCode = 501;
      }

    } else if (i == 1) {

      request.URIType = determineRequestURIForm(temp);

      if (request.URIType == ABS_URI) {
        separateResourceFromHostName(temp,request);
      } else {
        request.requestTarget = temp;
      }

    } else if (i == 2) {

      if (temp == "HTTP/1.0\r") {
        request.minorVersion = 0;
      } else if (temp != "HTTP/1.1\r") {
        //handle error
        request.errorCode = 403;
      }

    } else {
      //handle error of incorrect formatting
      request.errorCode = 400;
    }

    i++;
  } 
}

std::string ignoreLWS(const std::string &temp) {

  for (int i = 0; i < temp.size();i++) {
    if (temp[i] != ' ') {
      return temp.substr(i,temp.size()-i);
    }
  }

  return "";
}

//implement the host part in requestParser or in a separate file
void spliceHeaders(std::string &headerLine, request &request) {
  std::stringstream rawString(headerLine); //copy constructor in usage
  std::string temp;
  std::string buffer[2];
  int counter = 0;

  while (getline(rawString,temp,':')) {

    if (counter == 1) {
      if (temp[0] == ' ') {
        buffer[counter] = ignoreLWS(temp);
      } else {
        buffer[counter] = temp;
      }
      counter = 0;

      if (request.headers.contains("host") && buffer[0] == "host" && request.URIType != ABS_URI) {
        request.errorCode = 400;
      } else if (request.URIType != ABS_URI && buffer[0] == "host"){
        request.headers.insert({buffer[0],buffer[1].substr(0,buffer[1].length()-1)});
      } else {
        request.headers[buffer[0]] = buffer[1].substr(0,buffer[1].length()-1);
      }
      
    } else {
      turnHeaderToLowercase(temp);
      buffer[counter] = temp;
      counter += 1;
    }


  }

}



void requestParser(request &currentRequest) {
  std::stringstream rawString(currentRequest.msgBuffer);
  std::string temp;

  //body
  std::string rawBody;

  bool firstLine = true;
  bool atBody = false;
  
  while (getline(rawString,temp)) {

    if (firstLine) {

      if (temp != "" && temp != "\r") {

        firstLine = 0;
        //Handle with function of start line
        parseStartLine(temp,currentRequest);

      }
      //Ignore empty lines preceding firstLine

    } else if (!temp.size()) {

      //Handle next lines as body info by turning some variable true
      atBody = !atBody;

    } else {

      //handle body text and header meta data
      if (atBody) {
        rawBody += temp;
      } else {

        spliceHeaders(temp,currentRequest);

      }

    }
  }


  if (!currentRequest.headers.contains("host") && currentRequest.minorVersion == 1) {
    currentRequest.errorCode = 400;
  }
}