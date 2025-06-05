#include <string>

enum MIME : u_int8_t{
  TEXT_PLAIN,
  TEXT_HTML,
  TEXT_CSS,
  TEXT_JS,
  MESSAGE_HTTP,
  ERR,
};

MIME determineMIME(const std::string &invokedPath) {
  int length = invokedPath.size();

  if (length < 3) {
    return ERR;
  }

  if (invokedPath.substr(length-2,3) == ".js") {
    return TEXT_JS;
  } else if (invokedPath.substr(length-3,4) == ".css") {
    return TEXT_CSS;
  } else if (invokedPath.substr(length-3,4) == ".txt") {
    return TEXT_PLAIN;
  } else if (invokedPath.substr(length-4,5) == ".html") {
    return TEXT_HTML;
  } else {
    return TEXT_PLAIN;
  }

}

std::string getContentType(const MIME &MIMEtype) {
  if (TEXT_PLAIN) {
    return "text/plain";
  } else if (TEXT_HTML) {
    return "text/html";
  } else if (TEXT_CSS) {
    return "text/css";
  } else if (TEXT_JS) {
    return "text/javascript";
  } else if (MESSAGE_HTTP) {
    return "message/http";
  }
}