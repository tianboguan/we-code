#include <stdlib.h>
#include "cgi/lib/Login.h"
#include "thirdparty/glog/logging.h"
#include "common/sms/sms.h"
#include "common/encode/md5.h"
#include "common/redis_utils/RedisDefine.h"

Account::Account(const string &user, const string &token) {
  key_ = kAcountPrefix + user;
  user_ = user;
  token_ = token;
}

int Account::Enroll(const EnrollReq &enroll, string &token) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  if (account_info.code() != enroll.code()) {
    LOG(ERROR) << "enroll code invalid! user: " << user_
      << "provide: " << enroll.code()
      << "expect: " << account_info.code();
    return -1;
  }

  token = CreateToken();
  account_info.set_token(token);
  account_info.set_password(enroll.password());
  account_info.set_status(USER_STATUS_LOGIN);
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }
  return 0;
}

int Account::Login(const LoginReq &login, string &token) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  if (account_info.password() != login.password()) {
    LOG(ERROR) << "login passwd invalid! user: " << user_
      << "provide: " << login.password()
      << "expect: " << account_info.password();
    return -1;
  }

  token = CreateToken();
  account_info.set_token(token);
  account_info.set_status(USER_STATUS_LOGIN);
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  return 0;
}

int Account::Logout() {
  if (CheckLogin() != 0) {
    LOG(ERROR) << "user: " << user_ << "check login status failed!";
    return -1;
  }

  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  token_ = CreateToken();
  account_info.set_token(token_);
  account_info.set_status(USER_STATUS_LOGOUT);
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }
  return 0;
}

int Account::ModifyPass(const ModifyPassReq &modify_pass) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  if (account_info.password() != modify_pass.o_pass()) {
    LOG(ERROR) << "reset pass old password invalid! user: " << user_
      << "provide: " << modify_pass.o_pass()
      << "expect: " << account_info.password();
    return -1;
  }

  account_info.set_password(modify_pass.n_pass());
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  return 0;
}

int Account::ResetPass(const ResetPassReq &reset_pass) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret != RedisCodeOK) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  if (account_info.code() != reset_pass.code()) {
    LOG(ERROR) << "reset pass code invalid! user: " << user_
      << "provide: " << reset_pass.code()
      << "expect: " << account_info.code();
    return -1;
  }

  account_info.set_password(reset_pass.n_pass());
  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  return 0;
}

int Account::VerifyCode(const VerifyCodeReq &verify_code) {
  AccountInfo account_info;
  RedisCode ret = redis_.Get(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  string code = CreateCode();
  if (ret == RedisCodeKeyNotFound) {
    account_info.set_user(user_);
    account_info.set_password("");
    account_info.set_code(code);
    account_info.set_token("");
    account_info.set_status(USER_STATUS_ENROLL);
  } else {
    account_info.set_code(code);
  }

  ret = redis_.Set(key_, account_info);
  if (ret == RedisCodeError) {
    LOG(ERROR) << "Set account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  int sms_ret = SetPassSMS(user_, code);
  if (sms_ret != 0) {
    LOG(ERROR) << "send sms failed! user: " << user_;
    return -1;
  }

  return 0;
}

int Account::CheckLogin() {
  AccountInfo account_info;
  int ret = redis_.Get(key_, account_info);
  if (ret != 0) {
    LOG(ERROR) << "Get account info failed! user: "
      << user_ << ", err: " << redis_.Error();
    return -1;
  }

  if (token_ != account_info.token()) {
    LOG(ERROR) << "login token check failed! user: " << user_
      << ", invalid token: " << token_ 
      <<"valid token: " << account_info.token();
    return -1;
  }

  return 0;
}

string Account::Error() {
  return err_oss.str();
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
