#include "server.hpp"

void randomHTML(request &req, response &res) {
  std::string temp = "<!DOCTYPE html><head></head><body>Testing</body>";

  res.retrieveFile("index.html");
  //res.setBody(TEXT_HTML,temp);

};


void (*func)(request &req, response &res) = randomHTML;





int main() {
  server app = server("testing");

  app.get("/",func);

  app.get("/info",func);

  app.startListening();

  return 0;
}