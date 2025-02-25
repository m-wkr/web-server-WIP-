#include "server.hpp"

void randomHTML(request &req, response &res) {
  std::string temp = "<!DOCTYPE html><head></head><body>Testing</body>";

  resourceStatus temp1 = retrieveFile("index.html");

  if (temp1.statusCode == 200) {
    res.setBody(temp1.body);
  }
  res.addStatusCode(temp1.statusCode);

};

void (*func)(request &req, response &res) = randomHTML;





int main() {
  server app;

  app.manageConnection("/",func);

  app.startListening();

  return 0;
}