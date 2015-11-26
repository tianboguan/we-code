#include "cgi/lib/Profile.h"
#include "thirdparty/plog/Log.h"
#include "cgi/lib/CacheKeys.h"
#include "cgi/lib/CgiCode.h"
#include "common/tencent_img/TencentImg.h"

int Profile::Add(const UserProfile &profile) {
  RedisCode ret;
  int value;
  std::string key = kProfilePrefix + user_;
  ret = redis_.Query("EXISTS", key, &value);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "check profile exit or not failed! user: " << user_;
    return kCgiCodeSystemError;
  }

  if (value) {
    LOG_ERROR << user_ << "'s profile is exist, can't to create new one";
    return kCgiCodeRecreateUserProfile;
  }

  ret = profile_redis_.Query("SET", key, profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "set user profile failed! user: " << user_;
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int Profile::Alt(const UserProfile &profile) {
  RedisCode ret;
  UserProfile old_profile;
  std::string key = kProfilePrefix + user_;
  ret = profile_redis_.Query("GET", key, &old_profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user profile failed! user: " << user_; 
    return kCgiCodeSystemError;
  }

  old_profile.MergeFrom(profile);

  ret = profile_redis_.Query("SET", key, old_profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "update user profile failed! user: " << user_; 
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int Profile::AltHead(ImgRes *res) {
  TencentImg tencent_img;
  std::string file_id = tencent_img.GetFileId(user_, 1)[0];
  std::string download_url = tencent_img.GetUrl(file_id, DownloadImgUrl);

  res->set_appid(tencent_img.GetAppId());
  res->set_bucket(tencent_img.GetBucket());
  res->set_fileid(file_id);
  res->set_sign(tencent_img.GetPrivateSign(file_id));
  res->set_callback("patientsclub");

  RedisCode ret;
  UserProfile profile;
  std::string key = kProfilePrefix + user_;
  ret = profile_redis_.Query("GET", key, &profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user profile failed! user: " << user_;
    return kCgiCodeSystemError;
  }

  profile.set_new_head_url(download_url);
  ret = profile_redis_.Query("SET", key, profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "update user profile failed! user: " << user_; 
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int Profile::Query(const std::string &target_user, UserProfile *profile) {
  RedisCode ret;
  std::string key = kProfilePrefix + target_user;
  ret = profile_redis_.Query("GET", key, profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "read user profile failed! user: " << user_;
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}

int Profile::Query(const std::string &target_user, UserStat *stat) {
  RedisCode ret;
  std::string key = kStatPrefix + target_user;
  ret = stat_redis_.Query("GET", key, stat);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user stat failed! user: " << user_; 
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}

