#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/Recommand.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"

using namespace std;

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "/data/log/cgi/recommand.cgi.log");
  CgiHandle cgi_handle;
  map<string, string> params = cgi_handle.GetParams();

  ParseCgiReq parser(params);
  CgiBaseInfo base;
  int ret = parser.Parse(base);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << parser.Error();
    SendPostResWithoutData(ret);
    return 0;
  }

  Account account(base.user(), base.token()); 
  ret = account.CheckLogin();
  if (ret != kCgiCodeOk) {
    LOG_ERROR << "check login status for user " << base.user() << " failed!";
    SendPostResWithoutData(ret);
    return 0;
  }

  RecommandReq req;
  ret = parser.Parse(req);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << parser.Error();
    SendPostResWithoutData(ret);
    return 0;
  }

  RecommandRes res;
  Recommand recommand(base.user());
  ret = recommand.Get(req, &res);
  if (ret == kCgiCodeOk) {
    SendPostResWithData(ret, res);
  } else {
    SendPostResWithoutData(ret);
  }

  return 0;
}
