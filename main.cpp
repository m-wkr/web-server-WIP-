#include "server.hpp"

/*void randomHTML(request &req, response &res) {
  std::string temp = "<!DOCTYPE html><head></head><body>Testing</body>";

  res.statusCode = 200;
  res.retrieveFile("index.html");
  //res.setBody(TEXT_HTML,temp);

};


void (*func)(request &req, response &res) = randomHTML;*/





int main() {
  /*server app = server("testing");

  app.get("/",func);

  app.get("/info",func);

  app.post("/info",func);

  app.enableTrace("/");

  app.startListening();*/

  request exampleReq;

  requestParser(exampleReq);

  request eReq;

  parser(eReq);


  return 0;
}