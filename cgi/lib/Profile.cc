#include "cgi/lib/Profile.h"
#include "thirdparty/plog/Log.h"
#include "common/app/CgiCode.h"
#include "common/tencent_img/TencentImg.h"
#include "common/app/ProfileApi.h"
#include "common/app/StatisticApi.h"
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

int Profile::Query(const QueryProfileReq &req, UserProfile *profile) {
  ProfileApi profile_api;
  return profile_api.Get(req.target_user(), profile);
}

int Profile::Query(const std::string &target_user, UserStat *stat) {
  return StatisticApi::GetUserStat(target_user, stat);
}
