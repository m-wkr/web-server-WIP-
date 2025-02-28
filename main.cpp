#include "server.hpp"

void randomHTML(request &req, response &res) {
  std::string temp = "<!DOCTYPE html><head></head><body>Testing</body>";

  resourceStatus temp1 = retrieveFile("index.html");

  if (temp1.statusCode == 200) {
    res.setBody(temp1.body);
  }

};

//I'm thinking of this retrieveFile stuff being placed as a method in the response object, so res.retrieveFile() so we can handle the add body and status code procedures inside there

void (*func)(request &req, response &res) = randomHTML;





int main() {
  server app;

  app.manageConnection("/",func);

  app.manageConnection("/info",func);

  app.startListening();

  return 0;
}