#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Profile.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"

using namespace std;

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "/data/log/cgi/profile.cgi.log");
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

  Profile profile(base.user()); 
  string action = base.action();
  if (action == "alt" ) {
    AltProfileReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    ret = profile.Alt(req);
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "alt_head") {
    ImgRes  res;
    ret = profile.AltHead(&res);
    if (ret != kCgiCodeOk) {
      SendPostResWithoutData(ret);
    } else {
      SendPostResWithData(ret, res);
    }
    return 0;
  } else if (action == "query") {
    QueryProfileReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    UserProfile user_profile;
    ret = profile.Query(req, &user_profile);
    if (ret == kCgiCodeOk) {
      SendPostResWithData(ret, user_profile);
    } else {
      SendPostResWithoutData(ret);
    }
    return 0;
#if 0
  } else if (action == "stat") {
    QueryProfileReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    UserStat user_stat;
    ret = profile.Query(req.target_user(), &user_stat);
    if (ret == kCgiCodeOk) {
      SendPostResWithData(ret, user_stat);
    } else {
      SendPostResWithoutData(ret);
    }
    return 0;
#endif
  } else {
    LOG_ERROR << "invalid action info: " << action;
    SendPostResWithoutData(kCgiCodeParamError);
    return 0;
  }

  return 0;
}
