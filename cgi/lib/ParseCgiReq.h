#ifndef CGI_LIB_PARSECGIREQ_H_
#define CGI_LIB_PARSECGIREQ_H_

#include <map>
#include <sstream>
#include "proto/CgiReq.pb.h"

using namespace std;

class ParseCgiReq {
  public:
    ParseCgiReq(const map<string, string> &params) : params_(params) {}

    // base info
    int Parse(CgiBaseInfo &base);

    // account related
    int Parse(EnrollReq &enroll);
    int Parse(LoginReq &login);
    int Parse(ModifyPassReq &modify_pass);
    int Parse(ResetPassReq &reset_pass);
    int Parse(VerifyCodeReq &verify_code);

    string Error();

  private:
    bool Check(string key, bool escape = false);
    map<string, string> params_;
    ostringstream err_oss;
    int ret_;
};


#endif // CGI_LIB_PARSECGIREQ_H_
