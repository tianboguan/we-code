#ifndef CGI_LIB_PARSECGIREQ_H_
#define CGI_LIB_PARSECGIREQ_H_

#include <map>
#include <string>
#include <sstream>
#include "proto/CgiReq.pb.h"

// using namespace std;

class ParseCgiReq {
  public:
    ParseCgiReq(const std::map<std::string, std::string> &params) : params_(params) {}

    // base info
    int Parse(CgiBaseInfo &base);

    // account related
    int Parse(EnrollReq &enroll);
    int Parse(LoginReq &login);
    int Parse(ModifyPassReq &modify_pass);
    int Parse(ResetPassReq &reset_pass);
    int Parse(VerifyCodeReq &verify_code);

    // follow related
    int Parse(FollowReq &follow);

    // profile related
    int Parse(QueryProfileReq &query_profile);
    int Parse(AltProfileReq &alt_profile);

    // business related
    int Parse(AddressReq &address);
    int Parse(TagReq &tag);

    std::string Error();

  private:
    bool Check(std::string key, bool escape = false);
    std::map<std::string, std::string> params_;
    std::ostringstream err_oss;
    int ret_;
};


#endif // CGI_LIB_PARSECGIREQ_H_
