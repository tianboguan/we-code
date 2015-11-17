#include <iostream>
#include <fstream>
#include "thirdparty/glog/logging.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"
#include "common/utils/Pb2Json.h"

using namespace std;

const string kSystemErrorMsg = "系统异常";

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]); 
  FLAGS_log_dir = "/data/log/cgi";  
  FLAGS_logtostderr = false;
  FLAGS_stderrthreshold = 3;
  CgiHandle cgi_handle;
  map<string, string> params = cgi_handle.GetParams();

  string cgi_res_msg;
  int cgi_res_code;
  string cgi_res_data;

  ParseCgiReq parser(params);
  CgiBaseInfo base;
  int ret = parser.Parse(base);
  if (ret != 0) {
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
      SendPostResWithoutData(-1, kSystemErrorMsg);
    } else { 
      string token;
      ret = account.Enroll(enroll, token);
      cgi_res_code = ret;
      cgi_res_msg = account.Error();
      if (ret == 0) {
        res.set_token(token);
        SendPostResWithData(cgi_res_code, cgi_res_msg, res);
      }
      else {
        SendPostResWithoutData(cgi_res_code, cgi_res_msg);
      }
    }
    return 0;
  } else if (action == "login") {
    LoginReq login;
    AccountRes res;
    ret = parser.Parse(login);
    if (ret != 0) {
      SendPostResWithoutData(-1, kSystemErrorMsg);
    } else {
      string token;
      ret = account.Login(login, token);
      cgi_res_code = ret;
      cgi_res_msg = account.Error();
      if (ret == 0) {
        res.set_token(token);
        SendPostResWithData(cgi_res_code, cgi_res_msg, res);
      } else {
        SendPostResWithoutData(cgi_res_code, cgi_res_msg);
      }
    }
    return 0;
  } else if (action == "logout") {
    ret = account.Logout();
    cgi_res_code = ret;
    cgi_res_msg = account.Error();
    SendPostResWithoutData(cgi_res_code, cgi_res_msg);
    return 0;
  } else if (action == "mod_pass") {
    ModifyPassReq mod_pass;
    ret = parser.Parse(mod_pass);
    if (ret != 0) {
      SendPostResWithoutData(-1, kSystemErrorMsg);
    } else {
      ret = account.ModifyPass(mod_pass);
      cgi_res_code = ret;
      cgi_res_msg = account.Error();
      SendPostResWithoutData(cgi_res_code, cgi_res_msg);
    }
    return 0;
  } else if (action == "reset_pass") {
    ResetPassReq reset_pass;
    ret = parser.Parse(reset_pass);
    if (ret != 0) {
      SendPostResWithoutData(-1, kSystemErrorMsg);
    } else {
      ret = account.ResetPass(reset_pass);
      cgi_res_code = ret;
      cgi_res_msg = account.Error();
      SendPostResWithoutData(cgi_res_code, cgi_res_msg);
    }
    return 0;
  } else if (action == "send_code") {
    ret = account.SendCode();
    cgi_res_code = ret;
    cgi_res_msg = account.Error();
    SendPostResWithoutData(cgi_res_code, cgi_res_msg);
    return 0;
  } else if (action == "verify_code") {
    VerifyCodeReq verify_code;
    ret = parser.Parse(verify_code);
    if (ret != 0) {
      SendPostResWithoutData(-1, kSystemErrorMsg);
      return 0;
    } else {
      ret = account.VerifyCode(verify_code);
      cgi_res_code = ret;
      cgi_res_msg = account.Error();
      SendPostResWithoutData(cgi_res_code, cgi_res_msg);
    }
    return 0;
  } else {
    SendPostResWithoutData(-1, kSystemErrorMsg);
    return 0;
  }

  return 0;
}
