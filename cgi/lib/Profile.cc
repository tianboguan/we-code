#include "cgi/lib/Profile.h"
#include "thirdparty/plog/Log.h"
#include "cgi/lib/CacheKeys.h"
#include "common/tencent_img/TencentImg.h"

int Profile::Add(const UserProfile &profile) {
  RedisCode ret;
  int value;
  std::string key = kProfilePrefix + user_;
  ret = redis_.Query("EXISTS", key, &value);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "EXISTS " << key << " failed! err: " << redis_.Error();
    err_oss_ << "创建用户信息失败";
    return -1;
  }

  if (value) {
    LOG_ERROR << user_ << "'s profile is exist, can't to create new one";
    err_oss_ << "创建用户信息失败";
    return -1;
  }

  ret = profile_redis_.Query("SET", key, profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "SET " << key << " falied! err: " << redis_.Error();
    err_oss_ << "创建用户信息失败";
    return -1;
  }

  return 0;
}

int Profile::Alt(const UserProfile &profile) {
  RedisCode ret;
  UserProfile old_profile;
  std::string key = kProfilePrefix + user_;
  ret = profile_redis_.Query("GET", key, &old_profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "GET " << key << " falied! err: " << redis_.Error();
    err_oss_ << "修改用户信息失败";
    return -1;
  }

  old_profile.MergeFrom(profile);

  ret = profile_redis_.Query("SET", key, old_profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "SET " << key << " falied! err: " << redis_.Error();
    err_oss_ << "修改用户信息失败";
    return -1;
  }

  return 0;
}

int Profile::AltHead(AltHeadRes *res) {
  TencentImg tencent_img;
  std::string file_id;

  file_id = tencent_img.GetFileId(user_, 1)[0];
  std::string upload_url = tencent_img.GetUrl(file_id, UploadImgUrl);
  std::string download_url = tencent_img.GetUrl(file_id, DownloadImgUrl);
  std::string sign = tencent_img.GetPrivateSign(file_id);
  std::ostringstream callback;
  callback << "user=" << user_ << "|type=head|url=" << download_url; 
  res->set_head_url(upload_url);
  res->set_name(file_id);
  res->set_sign(sign);
  res->set_callback(callback.str());

  RedisCode ret;
  UserProfile profile;
  std::string key = kProfilePrefix + user_;
  ret = profile_redis_.Query("GET", key, &profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "GET " << key << " falied! err: " << redis_.Error();
    err_oss_ << "修改用户头像失败";
    return -1;
  }

  profile.set_new_head_url(download_url);
  ret = profile_redis_.Query("SET", key, profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "SET " << key << " falied! err: " << redis_.Error();
    err_oss_ << "修改用户头像失败";
    return -1;
  }

  return 0;
}

int Profile::Query(const std::string &target_user, UserProfile *profile) {
  RedisCode ret;
  std::string key = kProfilePrefix + target_user;
  ret = profile_redis_.Query("GET", key, profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "GET " << key << " falied! err: " << redis_.Error();
    err_oss_ << "查询用户资料失败";
    return -1;
  }
  return 0;
}

int Profile::Stat(const std::string &target_user, UserStat *stat) {
  RedisCode ret;
  std::string key = kStatPrefix + target_user;
  ret = stat_redis_.Query("GET", key, stat);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "GET " << key << " falied! err: " << redis_.Error();
    err_oss_ << "查询用户统计数据失败";
    return -1;
  }
  return 0;
}

std::string Profile::Error() {
  return 0;
}

