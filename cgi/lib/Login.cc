#include <stdlib.h>
#include <map>
#include <string>
#include "cgi/lib/Login.h"
#include "thirdparty/glog/logging.h"
#include "common/sms/sms.h"
#include "common/encode/md5.h"
#include "common/redis_utils/RedisDefine.h"
#include "common/utils/string_utils.h"

using namespace std;


Account::Account(const string &user, const string &token) {
  key_ = kAcountPrefix + user;
  user_ = user;
  token_ = token;
}

int Account::Enroll(const EnrollReq &enroll, string &token) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  } else if (ret == RedisCodeOK) {
    error_code_ = 1;
    return error_code_;   // there is already a user
  }

  token = CreateToken();
  account_info.set_user(user_);
  account_info.set_token(token);
  account_info.set_password(value_to_string(rand()));
  account_info.set_new_password(enroll.password());
  account_info.set_status(USER_STATUS_ENROLL);
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  }
  return 0;
}

int Account::Login(const LoginReq &login, string &token) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeError) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  } else if (ret == RedisCodeKeyNotFound) {
    error_code_ = 2;
    return error_code_;         // passwd error
  }

  if (account_info.password() != login.password()) {
    LOG(ERROR) << "login passwd invalid! user: " << user_
      << "provide: " << login.password()
      << "expect: " << account_info.password();
    error_code_ = 3;
    return error_code_;         // passwd error
  }

  token = CreateToken();
  account_info.set_token(token);
  account_info.set_status(USER_STATUS_LOGIN);
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  }

  return 0;
}

int Account::Logout() {
  if (CheckLogin() != 0) {
    LOG(ERROR) << "user: " << user_ << "check login status failed!";
    error_code_ = -1;
    return error_code_;
  }

  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  }

  token_ = CreateToken();
  account_info.set_token(token_);
  account_info.set_status(USER_STATUS_LOGOUT);
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  }
  return 0;
}

int Account::ModifyPass(const ModifyPassReq &modify_pass) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return error_code_;
  }

  if (account_info.password() != modify_pass.o_pass()) {
    LOG(ERROR) << "reset pass old password invalid! user: " << user_
      << "provide: " << modify_pass.o_pass()
      << "expect: " << account_info.password();
    error_code_ = 4;
    return error_code_;
  }

  account_info.set_password(modify_pass.n_pass());
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  }

  return 0;
}

int Account::ResetPass(const ResetPassReq &reset_pass) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  }

  account_info.set_new_password(reset_pass.n_pass());
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  }

  return 0;
}

int Account::SendCode() {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  }

  string code = CreateCode();
  account_info.set_code(code);
  ret = redis_.Set(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    error_code_ = -1;
    return error_code_;
  }

  if (SetPassSMS(user_, code) != 0) {
    LOG(ERROR) << "send sms failed! user: " << user_;
    error_code_ = 5;
    return error_code_;
  }

  return 0;
}

int Account::VerifyCode(const VerifyCodeReq &verify_code) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return error_code_;
  }

  if (account_info.code() != verify_code.code()) {
    LOG(ERROR) << "code not match! user: " << user_ 
      << "expect: " << account_info.code()
      << "provide: " << verify_code.code();
    error_code_ = 6;
    return error_code_;
  }

  account_info.set_status(USER_STATUS_LOGIN);
  account_info.set_password(account_info.new_password());
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return error_code_;
  }

  return 0;
}

int Account::CheckLogin() {
  AccountInfo account_info;
  int ret = redis_.Get(key_, account_info);
  if (ret != 0) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return error_code_;
  }

  if (token_ != account_info.token()
      || account_info.status() != USER_STATUS_LOGIN) {
    LOG(ERROR) << "check login failed! user: " << user_
      <<", expect token: " << account_info.token()
      << ", provide token: " << token_
      << ", user status: " << account_info.status();
    error_code_ = 7;
    return error_code_;
  }

  return 0;
}

string Account::Error() {
  static map<int, string> AccountError = {
    {0, "处理成功"},
    {-1, "系统错误"},
    {1, "该用户已经存在, 讲直接登录或是重置密码登录"},
    {2, "用户不存在，请先注册"},
    {3, "密码错误"},
    {4, "旧密码错误"},
    {5, "验证码发送失败, 请重新发送"},
    {6, "验证码错误"},
    {7, "请先登录"},
  };

  if (AccountError.find(error_code_) != AccountError.end()) {
    return AccountError[error_code_];
  } else {
    return "系统异常"; 
  }
}

string Account::CreateCode() {
  int code = rand() % 10000;
  char buf[6];
  bzero(buf, sizeof(buf));
  snprintf(buf, sizeof(buf), "%05d", code);
  return buf;
}

string Account::CreateToken() {
  char buf[32];
  bzero(buf, sizeof(buf));
  snprintf(buf, sizeof(buf), "%s%ld", user_.c_str(), time(NULL));
  MD5 md5(buf);
  return md5.toStr();
}
