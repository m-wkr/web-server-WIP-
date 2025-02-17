#include <iostream>
#include <string>
#include <sstream>
#include <map>

//WIP TO BE EXPANDED
enum requestTypes {
  GET,
  HEAD,
  PUT,
  POST
};

//best make request object

void parseStartLine(std::string &startLine,requestTypes &method, std::string &requestTarget) {
  std::stringstream rawString(startLine);
  std::string temp;

  int i = 0;

  while (getline(rawString,temp,' ')) {
    if (i == 0) {

      if (temp == "GET") {
        method = GET;
      } else if (temp == "HEAD") {
        method = HEAD;
      } else if (temp == "PUT") {
        method = PUT;
      } else if (temp == "POST") {
        method = POST;
      } else {
        //case sensitivity or invalid method to raise 501
      }

    } else if (i == 1) {

      requestTarget = temp;

    } else if (i == 2) {

      if (temp != "HTTP/1.1\r\n") {
        //handle error
      }

    } else {
      //handle error of incorrect formatting
    }

    i++;
  } 
}

void spliceHeaders(std::string &headerLine, std::map<std::string,std::string> &headerHashMap) {
  std::stringstream rawString(headerLine); //copy constructor in usage
  std::string temp;
  std::string buffer[2];
  int counter = 0;

  while (getline(rawString,temp,':')) {

    if (counter == 1) {
      if (temp[0] == ' ') {
        buffer[counter] = temp.substr(1,temp.size()-1);
      } else {
        buffer[counter] = temp;
      }
      counter = 0;
      headerHashMap.insert({buffer[0],buffer[1]});
    } else {
      buffer[counter] = temp;
      counter += 1;
    }


  }

}



void requestParser(const char* &&buffer) {
  std::stringstream rawString(buffer);
  std::string temp;

  //start line
  std::string startLine;

  requestTypes method;
  std::string requestTarget;



  //headers
  std::map<std::string,std::string> headers;

  //body
  std::string rawBody;


  bool firstLine = true;
  bool atBody = false;
  while (getline(rawString,temp)) {

    if(firstLine) {
      firstLine = 0;
      //Handle with function of start line
      startLine = temp;
      parseStartLine(startLine,method,requestTarget);

    } else if (!temp.size()) {

      //Handle next lines as body info by turning some variable true
      atBody = !atBody;

    } else {

      //handle body text and header meta data
      if (atBody) {
        rawBody += temp;
      } else {

        spliceHeaders(temp,headers);

      }

    }
  }

  //Debugging 
  std::cout << startLine << "\n";
  std::cout << method << "-" << requestTarget << '\n';

  std::map<std::string,std::string>::iterator i = headers.begin();
  while (i != headers.end()) {
    std::cout << i->first << "__" << i->second << '\n';
    i++;
  }

  std::cout << rawBody;

}