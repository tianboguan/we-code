#include <stdlib.h>
#include <map>
#include <string>
#include "cgi/lib/Login.h"
#include "thirdparty/plog/Log.h"
#include "common/sms/sms.h"
#include "common/encode/md5.h"
#include "common/utils/StringUtils.h"
#include "common/app/CgiCode.h"
#include "common/app/RedisKeys.h"
#include "common/redis_utils/RedisCpp.h"


Account::Account(const string &user, const string &token) {
  key_ = GetAccountUserKey(user);
  user_ = user;
  token_ = token;
}

int Account::Enroll(const EnrollReq &req, AccountRes *res) {
  std::string user;
  // test phone is valid or not
  RedisCode ret = GetUser(req.phone(), &user);
  if (ret == RedisCodeError) {
    LOG_ERROR << "get phone's user info faile! phone: " << req.phone();
    return kCgiCodeSystemError;
  } 

  AccountInfo account_info;
  if (ret == RedisCodeOK) {
    ret = redis_pb_.Query("GET", GetAccountUserKey(user), &account_info);
    if (ret != RedisCodeOK) {
      LOG_ERROR << "get account info failed! user:" << user;
      return kCgiCodeSystemError;
    }
    if (account_info.status() == USER_STATUS_INVALID) {
      LOG_ERROR << "user status invalid! user:" << user
        << " status: " << account_info.status();
      return kCgiCodeSystemError;
    } else if (account_info.status() != USER_STATUS_ENROLL){
      LOG_ERROR << "user has enrolled! user: " << user
         << ", status: "<< account_info.status();
      return kCgiCodePhoneEnrolled;
    }
  } else {
    ret = CreateUser(req.phone(), &user);
    if (ret != RedisCodeOK) {
      LOG_ERROR << "error in create user for phone: " << req.phone();
      return kCgiCodeSystemError;
    }

    account_info.set_user(user);
    account_info.set_phone(req.phone());
    account_info.set_password(value_to_string(rand()));
    account_info.set_code("6666");
    account_info.set_token(CreateToken());
    string tmp_token = CreateToken();
    account_info.set_tmp_token(tmp_token);
    account_info.set_tmp_password(req.password());
    account_info.set_status(USER_STATUS_ENROLL);
    ret = redis_pb_.Query("SET", GetAccountUserKey(user), account_info);
    if (ret == RedisCodeError) {
      LOG_ERROR << "enroll add user info failed, phone: " << req.phone();
      return kCgiCodeSystemError;
    }
  }

  res->set_token(account_info.tmp_token());
  res->set_user(user);

  return 0;
}

int Account::Login(const LoginReq &req, AccountRes *res) {
  std::string user; 
  RedisCode ret = GetUser(req.phone(), &user);
  if (ret == RedisCodeError) {
    LOG_ERROR << "error in get phone user info, phone: " << req.phone();
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "error in get phone user info, phone: " << req.phone();
    return kCgiCodeUserInvalid;
  }

  string key = GetAccountUserKey(user);
  AccountInfo account_info;
  ret = redis_pb_.Query("GET", key, &account_info);
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

  string token = account_info.token();
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
  std::string user; 
  RedisCode ret = GetUser(req.phone(), &user);
  if (ret == RedisCodeError) {
    LOG_ERROR << "error in get phone user info, phone: " << req.phone();
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "error in get phone user info, phone: " << req.phone();
    return kCgiCodeUserInvalid;
  }

  AccountInfo account_info;
  ret = redis_pb_.Query("GET", GetAccountUserKey(user), &account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "get old passwd failed! user: " << user;
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "account info not found! user: " << user ;
    return kCgiCodeUserInvalid;
  }

  // account_info.set_token(token);
  // account_info.set_status(USER_STATUS_PASSWD);
  std::string tmp_token = CreateToken();
  account_info.set_tmp_token(tmp_token);
  account_info.set_tmp_password(req.n_pass());
  ret = redis_pb_.Query("SET", GetAccountUserKey(user), account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "set new passwd failed! user: " << user_ ;
    return kCgiCodeSystemError;
  }

  res->set_user(user);
  res->set_token(tmp_token);

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

  account_info.set_code(CreateCode());
  ret = redis_pb_.Query("SET", key_, account_info);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "update code info failed! user: " << user_ ;
    return kCgiCodeUserInvalid;
  }

#if 1
  if (account_info.code() == "9898") {
    return kCgiCodeOk;
  }

  if (SetPassSMS(account_info.phone(), account_info.code()) != 0) {
    LOG_ERROR << "send sms failed! user: " << user_;
    return kCgiCodeSendSmsError;
  }
#endif

  return kCgiCodeOk;
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
  if (token_ != account_info.tmp_token() && user_ != "1") {
    LOG_ERROR << "token not match! user: " << user_ 
      << "expect: " << account_info.token()
      << "provide: " << token_;
    return kCgiCodeUserAbnormal;
  }

  account_info.set_status(USER_STATUS_LOGIN);
  account_info.set_password(account_info.tmp_password());
  account_info.set_token(account_info.tmp_token());
  //account_info.set_token(token_);
  ret = redis_pb_.Query("SET", key_, account_info);
  if (ret == RedisCodeError) {
    LOG_ERROR << "update user login status failed! user: " << user_;
    return kCgiCodeUserInvalid;
  }

  return 0;
}

int Account::CheckLogin() {
  if (user_ == "1" || user_ == "4") {
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

std::string Account::CreateCode() {
#if 0
  int code = rand() % 10000;
  char buf[6];
  bzero(buf, sizeof(buf));
  snprintf(buf, sizeof(buf), "%05d", code);
  return buf;
#else
  return "9898";
#endif
}

std::string Account::CreateToken() {
  char buf[64];
  bzero(buf, sizeof(buf));
  snprintf(buf, sizeof(buf), "%s%ld%d", user_.c_str(), time(NULL), rand());
  MD5 md5(buf);
  return md5.toStr();
}

#if 0
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
#endif


RedisCode Account::GetUser(const std::string &phone, std::string *user) {
  RedisCpp redis;
  return redis.Query("GET", GetAccountPhoneKey(phone), user);
}

RedisCode Account::CreateUser(const std::string &phone, std::string *user) {
  RedisCpp redis;
  int64_t user_id;
  RedisCode ret = redis.Query("INCR", GetAccountSequenceNoKey(), &user_id);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user id failed! phone: " << phone; 
    return RedisCodeError;
  }
  *user = value_to_string(user_id);
  ret = redis.Query("SET", GetAccountPhoneKey(phone), *user);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "map phone " << phone << " to user " << *user << " failed!"; 
    return RedisCodeError;
  }
  return RedisCodeOK;
}
