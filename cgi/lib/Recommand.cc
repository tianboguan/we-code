#include "cgi/lib/Recommand.h"
#include "thirdparty/plog/Log.h"
#include "common/app/CgiCode.h"
#include "common/app/ProfileApi.h"
#include "common/app/RecordApi.h"
#include "common/app/FollowApi.h"

int Recommand::Get(const RecommandReq &req, RecommandRes *res) {
  std::map<std::string, RoughRecord> records;
  RecordApi record_api;
  int ret_code = record_api.GetRecentRecord(user_, req.page(), &records);
  if (ret_code == kCgiCodeSystemError) {
    LOG_ERROR << "get home record failed! user: " << user_;
    return kCgiCodeSystemError;
  }

  std::set<std::string> users;
  std::map<std::string, RoughRecord>::iterator iter;
  for (iter = records.begin(); iter != records.end(); ++iter) {
    users.insert((iter->second).user());
  }
  std::map<std::string, UserProfile> profiles;
  ProfileApi profile_api;
  int ret = profile_api.MGet(users, &profiles);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << "get users profile failed! user: " << user_;
    return kCgiCodeSystemError;
  }
  
  FollowApi follow_api(user_);
  for (iter = records.begin(); iter != records.end(); ++iter) {
    RecommandUser recommand;
    std::string user = (iter->second).user();
    recommand.set_user(user);
    recommand.set_nickname(profiles[user].nickname());
    recommand.set_head(profiles[user].head());
    recommand.set_disease(profiles[user].disease());
    recommand.set_role(profiles[user].role());
    if (iter->second.pictures().size() > 0) {
      recommand.set_img((iter->second).pictures(0));
      LOG_ERROR << "add have picture record, " << iter->first;
    } else {
      LOG_ERROR << "ignore no picture record, " << iter->first;
      continue;
    }
    recommand.set_text((iter->second).text());
    bool followed = false;
    if (follow_api.IsFollowed(user, &followed) != kCgiCodeOk) {
      LOG_ERROR << "get " << user_ << " follow " << user << "status failed!";
      continue;
    }
    if (followed) {
      recommand.set_follow("yes");
    } else {
      recommand.set_follow("no");
    }

    *(res->add_users()) = recommand;
  }
  res->set_page(req.page());
  return ret_code;
}

