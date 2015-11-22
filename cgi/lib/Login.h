#ifndef CGI_LIB_LOGIN_H_
#define CGI_LIB_LOGIN_H_

#include <string>
#include <map>
#include <sstream>
#include "proto/CgiReq.pb.h"
#include "common/redis_utils/RedisPb.h"
#include "proto/CacheData.pb.h"
#include "cgi/lib/CacheKeys.h"

class Account{
  public:
    Account(const std::string &user, const std::string &token = ""); 

    int Enroll(const EnrollReq &enroll, std::string &token);
    int Login(const LoginReq &login, std::string &token);
    int Logout();
    int ModifyPass(const ModifyPassReq &modify_pass);
    int ResetPass(const ResetPassReq &reset_pass);
    int SendCode();
    int VerifyCode(const VerifyCodeReq &verify_code);
    int CheckLogin();

    std::string Error();
    std::string CreateCode();
    std::string CreateToken();

    int Read(AccountInfo &account_info);
    int Del();

  private:
    int error_code_;
    std::string key_;                 // redis key
    std::string user_;                 // redis key
    std::string token_;
    RedisStr2Pb<AccountInfo> redis_;
};


#endif // CGI_LIB_LOGIN_
