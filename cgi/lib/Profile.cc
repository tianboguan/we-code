#include "cgi/lib/Profile.h"
#include "thirdparty/plog/Log.h"
#include "common/app/CgiCode.h"
#include "common/tencent_img/TencentImg.h"
#include "common/app/ProfileApi.h"
//#include "common/utils/PbJsonUtil.h"
#include "common/pbjson/pbjson.h"


int Profile::Alt(const AltProfileReq &req) {
  UserProfile profile;
  std::string err;
  int ret = pbjson::json2pb(req.profile(), &profile, err);
  if (ret != 0) {
    LOG_ERROR << "json param parse failed! input: "
      << req.profile() << ", err: " << err << ", profile: " << profile.Utf8DebugString();
    return kCgiCodeParamError;
  }
  if (user_ != profile.user()) {
    LOG_ERROR << "user name not valid, user: " << user_ << ", profile:"
      << profile.DebugString();
    return kCgiCodeParamError;
  }

  LOG_ERROR << "alt profile: \n" << profile.DebugString();
  ProfileApi profile_api;
  return profile_api.Update(user_, profile);
}

int Profile::AltHead(ImgRes *res) {
  TencentImg tencent_img;
  std::string file_id = tencent_img.GetFileId(user_, 1)[0];
  std::string download_url = tencent_img.GetUrl(file_id, DownloadImgUrl);

  res->set_appid(tencent_img.GetAppId());
  res->set_bucket(tencent_img.GetBucket());
  res->set_fileid(file_id);
  res->set_sign(tencent_img.GetPublicSign());
  res->set_callback("patientsclub");

  UserProfile profile;
  profile.set_head(download_url);
  ProfileApi profile_api;
  return profile_api.Update(user_, profile);
}

// int Profile::Query(const std::string &target_user, UserProfile *profile) {
int Profile::Query(const QueryProfileReq &req, UserProfile *profile) {
  ProfileApi profile_api;
  return profile_api.Get(req.target_user(), profile);
}

#if 0
int Profile::Query(const std::string &target_user, UserStat *stat) {
  std::string key = GetUserStatKey(user_);
    RedisCode Query(std::string cmd, const std::string &key, 
                const std::string &param1, const std::string &param2);
  RedisCpp redis;
  std::map<std::string, std::string> stat_kv;
  RedisCode ret = redis.Query("HGETALL", key, &stat_kv);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user stat failed! user: " << user_; 
    return kCgiCodeSystemError;
  }
  stat->set_user(target_user);
  stat->set_login(atoi(stat_kv["login"].c_str()));
  stat->set_follow(atoi(stat_kv["follow"].c_str()));
  stat->set_followed(atoi(stat_kv["followed"].c_str()));
  stat->set_record(atoi(stat_kv["record"].c_str()));
  stat->set_comment(atoi(stat_kv["comment"].c_str()));
  stat->set_commented(atoi(stat_kv["commented"].c_str()));
  stat->set_like(atoi(stat_kv["like"].c_str()));
  stat->set_liked(atoi(stat_kv["liked"].c_str()));

  return kCgiCodeOk;
}
#endif
