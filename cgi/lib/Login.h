#ifndef CGI_LIB_LOGIN_H_
#define CGI_LIB_LOGIN_H_

#include <string>
#include <map>
#include <sstream>
#include "proto/CgiReq.pb.h"
#include "common/redis_utils/RedisPb.h"
#include "proto/CacheData.pb.h"

class Account{
  public:
    Account(const std::string &user, const std::string &token = ""); 

    int Enroll(const EnrollReq &req, AccountRes *res);
    int Login(const LoginReq &req, AccountRes *res);
    int Logout();
    int ModifyPass(const ModifyPassReq &modify_pass);
    int ResetPass(const ResetPassReq &reset_pass, AccountRes *res);
    int SendCode();
    int VerifyCode(const VerifyCodeReq &verify_code);
    int CheckLogin();

    int Read(AccountInfo &account_info);
    int Del();

  private:
    string GetUserId(const string &phone);
    std::string CreateCode();
    std::string CreateToken();

  private:
    int error_code_;
    std::string key_;                 // redis key
    std::string user_;                 // redis key
    std::string token_;
    RedisStr2Pb<AccountInfo> redis_pb_;
    RedisCpp redis_;
};


#endif // CGI_LIB_LOGIN_
