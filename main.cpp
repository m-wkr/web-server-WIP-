#include "server.hpp"

void randomHTML(request &req, response &res) {
  std::string temp = "<!DOCTYPE html><head></head><body>Testing</body>";

  std::string fileContent = retrieveFile("index.html").body;
  res.setBody(
    fileContent
  );
};

void (*func)(request &req, response &res) = randomHTML;





int main() {
  server app;

  //app.addFileHandler("/","index.html"); 

  app.manageConnection("/",func);

  app.startListening();

  return 0;
}