#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "requests.hpp"

void turnHeaderToLowercase(std::string &header) {
  std::transform(header.begin(),header.end(),header.begin(),[](unsigned char c) { return std::tolower(c); });
}

void parseStartLine(std::string &startLine, request &request) {
  std::stringstream rawString(startLine);
  std::string temp;

  int i = 0;

  while (getline(rawString,temp,' ')) {
    if (i == 0) {

      if (temp == "GET") {
        request.method = GET;
      } else if (temp == "HEAD") {
        request.method = HEAD;
      } else if (temp == "PUT") {
        request.method = PUT;
      } else if (temp == "POST") {
        request.method = POST;
      } else {
        request.errorCode = 405;
      }

    } else if (i == 1) {

      request.requestTarget = temp;

    } else if (i == 2) {
      if (temp != "HTTP/1.1\r") {
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
      request.headers.insert({buffer[0],buffer[1]});
    } else {
      turnHeaderToLowercase(temp);
      buffer[counter] = temp;
      counter += 1;
    }


  }

}



void requestParser(const char* &&buffer, request &currentRequest) {
  std::stringstream rawString(buffer);
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

  //Debugging 
  std::cout << currentRequest.method << "-" << currentRequest.requestTarget << '\n';

  std::map<std::string,std::string>::iterator i = currentRequest.headers.begin();
  while (i != currentRequest.headers.end()) {
    std::cout << i->first << "__" << i->second << '\n';
    i++;
  }

  std::cout << rawBody;

}