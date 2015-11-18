#include <iostream>
#include <fstream>
// #include "thirdparty/glog/logging.h"
#include "thirdparty/plog/Log.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"
#include "common/utils/Pb2Json.h"

using namespace std;

const string kSystemErrorMsg = "系统异常";

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "/data/log/cgi/account.cgi.log");
  CgiHandle cgi_handle;
  map<string, string> params = cgi_handle.GetParams();

  ParseCgiReq parser(params);
  CgiBaseInfo base;
  int ret = parser.Parse(base);
  if (ret != 0) {
    LOG_ERROR << parser.Error();
    SendPostResWithoutData(-1, kSystemErrorMsg);
    return 0;
  }

  Account account(base.user(), base.token()); 
  string action = base.action();
  if (action == "enroll" ) {
    EnrollReq enroll;
    AccountRes res;
    ret = parser.Parse(enroll);
    if (ret != 0) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(-1, kSystemErrorMsg);
    } else { 
      string token;
      ret = account.Enroll(enroll, token);
      if (ret == 0) {
        res.set_token(token);
        SendPostResWithData(0, account.Error(), res);
      }
      else {
        SendPostResWithoutData(ret, account.Error());
      }
    }
    return 0;
  } else if (action == "login") {
    LoginReq login;
    AccountRes res;
    ret = parser.Parse(login);
    if (ret != 0) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(-1, kSystemErrorMsg);
    } else {
      string token;
      ret = account.Login(login, token);
      if (ret == 0) {
        res.set_token(token);
        SendPostResWithData(ret, account.Error(), res);
      } else {
        SendPostResWithoutData(ret, account.Error());
      }
    }
    return 0;
  } else if (action == "logout") {
    ret = account.Logout();
    SendPostResWithoutData(ret, account.Error());
    return 0;
  } else if (action == "mod_pass") {
    ModifyPassReq mod_pass;
    ret = parser.Parse(mod_pass);
    if (ret != 0) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(-1, kSystemErrorMsg);
    } else {
      ret = account.ModifyPass(mod_pass);
      SendPostResWithoutData(ret, account.Error());
    }
    return 0;
  } else if (action == "reset_pass") {
    ResetPassReq reset_pass;
    ret = parser.Parse(reset_pass);
    if (ret != 0) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(-1, kSystemErrorMsg);
    } else {
      ret = account.ResetPass(reset_pass);
      SendPostResWithoutData(ret, account.Error());
    }
    return 0;
  } else if (action == "send_code") {
    ret = account.SendCode();
    SendPostResWithoutData(ret, account.Error());
    return 0;
  } else if (action == "verify_code") {
    VerifyCodeReq verify_code;
    ret = parser.Parse(verify_code);
    if (ret != 0) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(-1, kSystemErrorMsg);
      return 0;
    } else {
      ret = account.VerifyCode(verify_code);
      SendPostResWithoutData(ret, account.Error());
    }
    return 0;
  } else {
    LOG_ERROR << "invalid action info: " << action;
    SendPostResWithoutData(-1, kSystemErrorMsg);
    return 0;
  }

  return 0;
}
