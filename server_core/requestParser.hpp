#include "requestEnums.hpp"
#include "requestHelper.hpp"

enum parseState : u_int8_t {
  REQLINE,
  HEADER,
  HEADERCONTENT,
  BEGINBODY,
  BODY
};


void parser(request &cRequest);