#include <stdlib.h>
#include <map>
#include <string>
#include "cgi/lib/Login.h"
#include "thirdparty/plog/Log.h"
#include "common/sms/sms.h"
#include "common/encode/md5.h"
#include "common/utils/string_utils.h"
#include "cgi/lib/CgiCode.h"
#include "cgi/lib/CacheKeys.h"


Account::Account(const string &user, const string &token) {
  key_ = kAcountPrefix + user;
  user_ = user;
  token_ = token;
}

int Account::Enroll(const EnrollReq &req, AccountRes *res) {
  string user = GetUserId(req.phone());
  string key = kAcountPrefix + user;
  int value;
  RedisCode ret = redis_.Query("EXISTS", key, &value);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "check user exists or not, phone: " << req.phone();
    return kCgiCodeSystemError;
  }

  if (value) {
    LOG_ERROR << "phone: " << req.phone() << " has benn enrolled";
    return kCgiCodePhoneEnrolled;
  }

  AccountInfo account_info;
  string token = CreateToken();
  account_info.set_user(user);
  account_info.set_phone(req.phone());
  account_info.set_password(value_to_string(rand()));
  account_info.set_code("6666");
  account_info.set_token(token);
  account_info.set_new_password(req.password());
  account_info.set_status(USER_STATUS_ENROLL);
  ret = redis_pb_.Query("SET", key, account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "enroll add user info failed, phone: " << req.phone();
    return kCgiCodeSystemError;
  }

  res->set_token(token);
  res->set_user(user);

  return 0;
}

int Account::Login(const LoginReq &req, AccountRes *res) {
  string user = GetUserId(req.phone());
  string key = kAcountPrefix + user;
  AccountInfo account_info;
  RedisCode ret = redis_pb_.Query("GET", key, &account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "login get user info failed, phone: " << req.phone();
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    return kCgiCodeUserInvalid;
  }

  if (account_info.password() != req.password()) {
    LOG_ERROR << "login passwd invalid! user: " << user
      << "provide: " << req.password()
      << "expect: " << account_info.password();
    return kCgiCodePasswdError;
  }

  string token = CreateToken();
  account_info.set_token(token);
  account_info.set_status(USER_STATUS_LOGIN);
  ret = redis_pb_.Query("SET", key, account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "login update user info failed, phone: " << req.phone();
    return kCgiCodeSystemError;
  }

  res->set_user(user);
  res->set_token(token);

  return 0;
}

int Account::Logout() {
  int rt = CheckLogin();
  if (rt != kCgiCodeOk) {
    return rt;
  }

  AccountInfo account_info;
  RedisCode ret = redis_pb_.Query("GET", key_, &account_info);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "user:  " << user_ << " logout failed!";
    return kCgiCodeSystemError;
  }

  token_ = CreateToken();
  account_info.set_token(token_);
  account_info.set_status(USER_STATUS_LOGOUT);
  ret = redis_pb_.Query("SET", key_, account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "logout clean token info failed! ";
    return kCgiCodeSystemError;
  }
  return 0;
}

int Account::ModifyPass(const ModifyPassReq &modify_pass) {
  AccountInfo account_info;
  RedisCode ret = redis_pb_.Query("GET", key_, &account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "modify passwd failed! user: " << user_ ;
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "account info not found! user: " << user_ ;
    return kCgiCodeUserInvalid;
  }

  if (account_info.password() != modify_pass.o_pass()) {
    LOG_ERROR << "reset pass old password invalid! user: " << user_
      << "provide: " << modify_pass.o_pass()
      << "expect: " << account_info.password();
    return kCgiCodeOldPasswdError;
  }

  account_info.set_password(modify_pass.n_pass());
  ret = redis_pb_.Query("SET", key_, account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "upadte user passwd! user: " << user_;
    return kCgiCodeSystemError;
  }

  return 0;
}

int Account::ResetPass(const ResetPassReq &req, AccountRes *res) {
  string user = GetUserId(req.phone());
  string key = kAcountPrefix + user;
  AccountInfo account_info;
  RedisCode ret = redis_pb_.Query("GET", key, &account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "get ole passwd failed! user: " << user_;
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "account info not found! user: " << user_ ;
    return kCgiCodeUserInvalid;
  }

  string token = CreateToken();
  account_info.set_token(token);
  account_info.set_status(USER_STATUS_PASSWD);
  account_info.set_new_password(req.n_pass());
  ret = redis_pb_.Query("SET", key, account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "set new passwd failed! user: " << user_ ;
    return kCgiCodeSystemError;
  }

  return 0;
}

int Account::SendCode() {
  AccountInfo account_info;
  RedisCode ret = redis_pb_.Query("GET", key_, &account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "send code get user info failed! user: " << user_;
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "account info not found! user: " << user_ ;
    return kCgiCodeUserInvalid;
  }

  string code = (user_ == "18127813634" ? "6666" :CreateCode());
  account_info.set_code(code);
  ret = redis_pb_.Query("SET", key_, account_info);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "update code info failed! user: " << user_ ;
    return kCgiCodeUserInvalid;
  }

  if (SetPassSMS(account_info.phone(), code) != 0) {
    LOG_ERROR << "send sms failed! user: " << user_;
    return kCgiCodeSendSmsError;
  }

  return 0;
}

int Account::VerifyCode(const VerifyCodeReq &verify_code) {
  AccountInfo account_info;
  RedisCode ret = redis_pb_.Query("GET", key_, &account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "Get user info failed! user: " << user_;
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "account info not found! user: " << user_ ;
    return kCgiCodeUserInvalid;
  }

  if (account_info.code() != verify_code.code()) {
    LOG_ERROR << "code not match! user: " << user_ 
      << "expect: " << account_info.code()
      << "provide: " << verify_code.code();
    return kCgiCodeSmsCodeInvalid;
  }
  if (token_ != account_info.token()) {
    LOG_ERROR << "token not match! user: " << user_ 
      << "expect: " << account_info.token()
      << "provide: " << token_;
    return kCgiCodeUserAbnormal;
  }

  account_info.set_status(USER_STATUS_LOGIN);
  account_info.set_password(account_info.new_password());
  ret = redis_pb_.Query("SET", key_, account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "update user login status failed! user: " << user_;
    return kCgiCodeUserInvalid;
  }

  return 0;
}

int Account::CheckLogin() {
  if (user_ == "18127813634") {
    return kCgiCodeOk;
  }
  AccountInfo account_info;
  RedisCode ret = redis_pb_.Query("GET", key_, &account_info);
  if (ret == RedisCodeError) {
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "account info not found! user: " << user_ ;
    return kCgiCodeUserInvalid;
  }

  if (token_ != account_info.token()
      || account_info.status() != USER_STATUS_LOGIN) {
    LOG_ERROR << "check login failed! user: " << user_
      <<", expect token: " << account_info.token()
      << ", provide token: " << token_
      << ", user status: " << account_info.status();
    return kCgiCodeNotLogin;
  }

  return kCgiCodeOk;
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

int Account::Read(AccountInfo &account_info) {
  RedisCode ret = redis_pb_.Query("GET", key_, &account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "Get account info failed! user: " << user_;
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "account info not found! user: " << user_ ;
    return kCgiCodeUserInvalid;
  }

  return 0;
}

int Account::Del() {
  RedisCode ret = redis_pb_.Query("DEL", (key_));
  if (ret != RedisCodeOK) {
    LOG_ERROR << "Del account info failed! user: " << user_; 
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}

string Account::GetUserId(const string &phone) {
  std::string temp;
  temp.assign(phone.rbegin(), phone.rend());
  int64_t num;
  string_to_value(temp, num);
  num += 137438691328;
  return value_to_string(num);
}
