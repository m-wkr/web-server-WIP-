#include "server.hpp"

int main() {
  server app;

  app.addFileHandler("/","index.html"); 

  app.startListening();

  return 0;
}