#ifndef CGI_LIB_PARSECGIREQ_H_
#define CGI_LIB_PARSECGIREQ_H_

#include <map>
#include <string>
#include <sstream>
#include "proto/CgiReq.pb.h"

class ParseCgiReq {
  public:
    ParseCgiReq(const std::map<std::string, std::string> &params) {
      params_ = params;
      if (params_.find("user") == params_.end()) {
        params_["user"] = "patientsclub";
      }
      if (params_.find("token") == params_.end()) {
        params_["token"] = "patientsclub";
      }
    }

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

    // interact related
    int Parse(LikeReq &like);
    int Parse(UnlikeReq &unlike);
    int Parse(CommentReq &comment);
    int Parse(UncommentReq &uncomment);
    int Parse(RecordInteractReq &record_interact);
    int Parse(UserInteractReq &user_interact);

    // record related
    int Parse(CreateRecordReq &req);
    int Parse(DelRecordReq &req);
    int Parse(QueryRecordReq &req);
    int Parse(QueryRecordListReq &req);
    int Parse(AltRecordQrivateReq &req);

    // recommand related
    int Parse(RecommandReq &req);

    // feedback related
    int Parse(FeedbackReq &req);

    std::string Error();

  private:
    bool Check(std::string key, bool need = false);
    std::map<std::string, std::string> params_;
    std::ostringstream err_oss;
    int ret_;
};


#endif // CGI_LIB_PARSECGIREQ_H_
