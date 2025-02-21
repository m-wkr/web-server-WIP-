#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>

//Serving static content

struct resourceStatus {
  int statusCode = 200;
  std::string body;
};

//idk about the name chief :/ VERY MASSIVE WIP
class resourceHandler {
  std::map<std::string,std::string> fileHandlers = {};

  public:
  // Add a resource handler - currently only files
  void addFileHandler(std::string path, std::string filePath) {
      fileHandlers[path] = filePath;
  }

  resourceStatus retrieveResource(std::string invokedPath) {
    resourceStatus currentResource;

    try {
      std::ifstream targetFile(fileHandlers.at(invokedPath));
      std::string temp;

      while (getline(targetFile,temp)) {
        currentResource.body += temp;
      }

      targetFile.close();
    }
    catch (...) {
      currentResource.statusCode = 404;
    }

    return currentResource;
  }

};