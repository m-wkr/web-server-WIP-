#include "requestHelper.hpp"

enum parseState : u_int8_t {
  REQLINE_METHOD,
  REQLINE_URI,
  REQLINE_HTTP_VER,
  HEADER,
  HEADER_CONTENT,
  BEGIN_BODY,
  BODY
};


void parser(request &cRequest);