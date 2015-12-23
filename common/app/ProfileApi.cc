#include "common/app/ProfileApi.h"
#include "thirdparty/plog/Log.h"
#include "common/app/CgiCode.h"
#include "common/app/RedisKeys.h"
#include "common/redis_utils/RedisPb.h"
#include "common/redis_utils/RedisCpp.h"

int ProfileApi::Create(const std::string &user, bool check) {
  std::string key = GetUserProfileKey(user);
  RedisCpp redis;
  if (check) {
    int64_t value;
    RedisCode ret = redis.Query("EXISTS", key, &value);
    if (ret != RedisCodeOK) {
      LOG_ERROR << "check profile exit or not failed! user: " << user;
      return kCgiCodeSystemError;
    } 
    if (value != 0) {
      LOG_ERROR << user << "'s profile is exist, can't to create new one";
      return kCgiCodeRecreateUserProfile;
    }
  }

  RedisStr2Pb<UserProfile> redis_pb;
  UserProfile profile;
  profile.set_user(user);
  profile.set_enrolltime(time(NULL));
  RedisCode ret = redis_pb.Query("SET", key, profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "set user profile failed! user: " << user;
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int ProfileApi::Create(const UserProfile &profile, bool check) {
  int ret = Create(profile.user(), check);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << "create user failed! user: " << profile.user();
    return ret;
  }

  return Update(profile.user(), profile);
}

int ProfileApi::Update(const std::string &user, const UserProfile &profile) {
  UserProfile old_profile;
  RedisStr2Pb<UserProfile> redis_pb;
  std::string key = GetUserProfileKey(user);
  RedisCode ret = redis_pb.Query("GET", key, &old_profile);
  if (ret == RedisCodeError) {
    LOG_ERROR << "get user profile failed! user: " << user; 
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil){
    old_profile.set_user(user);
    old_profile.set_enrolltime(time(NULL));
  } 

  old_profile.MergeFrom(profile);

  LOG_ERROR << "set profile: "; 
  LOG_ERROR << profile.DebugString();
  LOG_ERROR << "set old profile: "; 
  LOG_ERROR << old_profile.DebugString();
  ret = redis_pb.Query("SET", key, old_profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "update user profile failed! user: " << user; 
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int ProfileApi::Get(const std::string &user, UserProfile *profile) {
  RedisStr2Pb<UserProfile> redis_pb;
  RedisCode ret = redis_pb.Query("GET", GetUserProfileKey(user), profile);
  if (ret == RedisCodeError) {
    LOG_ERROR << "get user profile failed! user: " << user; 
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "please set user profile first! user: " << user; 
    return kCgiCodeSetUserProfile;
  }

  return kCgiCodeOk;
}

int ProfileApi::Get(const std::string &user, StripUserProfile *base) {
  UserProfile profile;
  int ret = Get(user, &profile);
  if (ret != kCgiCodeOk) {
    return ret;
  }
  Profile2Base(profile, base);
  return kCgiCodeOk;
}
#if 0
std::vector<std::string> keys;
std::vector<UserProfile> values;

for (std::vector<std::string>::const_iterator iter = users.begin();
    iter != users.end(); ++iter) {
  keys.push_back(key_prefix + *iter);
}

RedisCode ret = profile_redis_.Query("MGET", keys, &values);
if (ret != RedisCodeOK) {
  LOG_ERROR << "get " << user_ << " follow list failed!";
  return kCgiCodeSystemError;
}

for (std::vector<UserProfile>::const_iterator iter = values.begin();
    iter != values.end(); ++iter) {
  UserProfile* user = res->add_users();
  *user = *iter;
}
#endif

int ProfileApi::MGet(const std::set<std::string> &users,
    std::map<std::string, UserProfile> *profiles) {
  std::vector<std::string> keys;
  for (std::set<std::string>::const_iterator iter = users.begin();
      iter != users.end(); ++iter) {
    keys.push_back(GetUserProfileKey(*iter));
  }

  RedisStr2Pb<UserProfile> redis_pb;
  if (redis_pb.Query("MGET", keys, profiles) != RedisCodeOK) {
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int ProfileApi::MGet(const std::set<std::string> &users,
    std::map<std::string, StripUserProfile> *strips) {
  std::map<std::string, UserProfile> profiles;
  int ret  = MGet(users, &profiles);
  if (ret != kCgiCodeOk) {
    return ret;
  }

  for (std::map<std::string, UserProfile>::iterator iter = profiles.begin();
      iter != profiles.end(); ++iter) {
    StripUserProfile strip;
    Profile2Base(iter->second, &((*strips)[iter->first]));
  }
  return kCgiCodeOk;
}

int ProfileApi::Del(const std::string &user) {
  RedisCpp redis;
  return (redis.Query("DEL", GetUserProfileKey(user)) == RedisCodeOK)
    ? kCgiCodeOk : kCgiCodeSystemError;
}

#if 0
int ProfileApi::Get(const std::string &user, UserStat *stat) {
  std::string key = GetUserStatKey(user);
  RedisCode Query(std::string cmd, const std::string &key, 
      const std::string &param1, const std::string &param2);
  RedisCpp redis;
  std::map<std::string, std::string> stat_kv;
  RedisCode ret = redis.Query("HGETALL", key, &stat_kv);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user stat failed! user: " << user; 
    return kCgiCodeSystemError;
  }
  stat->set_user(user);
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

void ProfileApi::Profile2Base(const UserProfile &profile, StripUserProfile *base) {
  base->set_user(profile.user());
  base->set_nickname(profile.nickname());
  base->set_head(profile.head());
}
