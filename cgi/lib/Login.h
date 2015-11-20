#ifndef CGI_LIB_LOGIN_H_
#define CGI_LIB_LOGIN_H_

#include <string>
#include <map>
#include <sstream>
#include "proto/CgiReq.pb.h"
#include "common/redis_utils/RedisPb.h"
#include "proto/CacheData.pb.h"

using namespace std;

const string kAcountPrefix = "account_";

class Account{
  public:
    Account(const string &user, const string &token = ""); 

    int Enroll(const EnrollReq &enroll, string &token);
    int Login(const LoginReq &login, string &token);
    int Logout();
    int ModifyPass(const ModifyPassReq &modify_pass);
    int ResetPass(const ResetPassReq &reset_pass);
    int SendCode();
    int VerifyCode(const VerifyCodeReq &verify_code);
    int CheckLogin();

    string Error();
    string CreateCode();
    string CreateToken();

    int Read(AccountInfo &account_info);
    int Del();

  private:
    int error_code_;
    string key_;                 // redis key
    string user_;                 // redis key
    string token_;
    RedisStr2Pb<AccountInfo> redis_;
};


#endif // CGI_LIB_LOGIN_
