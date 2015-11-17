#include "cgi/lib/ParseCgiReq.h"

#define CHECK(key, escape)  \
ret_ = Check(key, escape);   \
if (!ret_) {                 \
  return -1;                \
}

int ParseCgiReq::Parse(CgiBaseInfo &base) {
  CHECK("user", true);
  CHECK("token", false);
  CHECK("action", false);
  base.set_user(params_["user"]);
  base.set_token(params_["token"]);
  base.set_action(params_["action"]);
  return 0;
}

// account related
int ParseCgiReq::Parse(EnrollReq &enroll) {
  CHECK("user", true);
  CHECK("password", true);
  CHECK("code", true);
  enroll.set_user(params_["user"]);
  enroll.set_password(params_["password"]);
  return 0;
}

int ParseCgiReq::Parse(LoginReq &login) {
  CHECK("user", true);
  CHECK("password", true);
  login.set_user(params_["user"]);
  login.set_password(params_["password"]);
  return 0;
}

int ParseCgiReq::Parse(ModifyPassReq &modify_pass) {
  CHECK("o_pass", true);
  CHECK("n_pass", true);
  modify_pass.set_o_pass(params_["o_pass"]);
  modify_pass.set_n_pass(params_["n_pass"]);
  return 0;
}

int ParseCgiReq::Parse(ResetPassReq &reset_pass) {
  CHECK("code", true);
  CHECK("n_pass", true);
  reset_pass.set_n_pass(params_["n_pass"]);
  return 0;
}

int ParseCgiReq::Parse(VerifyCodeReq &verify_code) {
  CHECK("phone", true);
  verify_code.set_code(params_["code"]);
  return 0;
}

string ParseCgiReq::Error() {
  return err_oss.str();
}

bool ParseCgiReq::Check(string key, bool escape) {
  if (params_.find(key) == params_.end()) {
    return false;
  }
  if (escape) {
    // do escape
  }
  return true;
}

