#include "server.hpp"

void randomHTML(request &req, response &res) {
  std::string temp = "<!DOCTYPE html><head></head><body>Testing</body>";
  res.setBody(
    temp
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