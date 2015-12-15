#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"

using namespace std;

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "/data/log/cgi/account.cgi.log");
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
  string action = base.action();
  if (action == "enroll" ) {
    EnrollReq enroll;
    AccountRes res;
    ret = parser.Parse(enroll);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
    } else { 
      ret = account.Enroll(enroll, &res);
      if (ret == kCgiCodeOk) {
        SendPostResWithData(ret, res);
      }
      else {
        SendPostResWithoutData(ret);
      }
    }
    return 0;
  } else if (action == "login") {
    LoginReq login;
    AccountRes res;
    ret = parser.Parse(login);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
    } else {
      ret = account.Login(login, &res);
      if (ret == kCgiCodeOk) {
        SendPostResWithData(ret, res);
      } else {
        SendPostResWithoutData(ret);
      }
    }
    return 0;
  } else if (action == "logout") {
    ret = account.Logout();
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "mod_pass") {
    ModifyPassReq mod_pass;
    ret = parser.Parse(mod_pass);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
    } else {
      ret = account.ModifyPass(mod_pass);
      SendPostResWithoutData(ret);
    }
    return 0;
  } else if (action == "reset_pass") {
    ResetPassReq reset_pass;
    AccountRes res;
    ret = parser.Parse(reset_pass);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
    } else {
      ret = account.ResetPass(reset_pass, &res);
      if (ret != kCgiCodeOk) {
        SendPostResWithoutData(ret);
      } else {
        SendPostResWithData(ret, res);
      }
    }
    return 0;
  } else if (action == "send_code") {
    ret = account.SendCode();
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "verify_code") {
    VerifyCodeReq verify_code;
    ret = parser.Parse(verify_code);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    } else {
      ret = account.VerifyCode(verify_code);
      SendPostResWithoutData(ret);
    }
    return 0;
  } else {
    LOG_ERROR << "invalid action info: " << action;
    SendPostResWithoutData(kCgiCodeParamError);
    return 0;
  }

  return 0;
}
