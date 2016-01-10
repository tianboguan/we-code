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
  int ret = Get(user, &old_profile);
  if (ret == kCgiCodeSystemError) {
    LOG_ERROR << "get user profile failed! user: " << user; 
    return kCgiCodeSystemError;
  } else if (ret == kCgiCodeUserProfileInexist) {
    old_profile.set_user(user);
    old_profile.set_enrolltime(time(NULL));
    old_profile.set_start_time(time(NULL));
  }

  old_profile.MergeFrom(profile);
  return Set(user, old_profile);
}

int ProfileApi::Get(const std::string &user, UserProfile *profile) {
  RedisStr2Pb<UserProfile> redis_pb;
  RedisCode ret = redis_pb.Query("GET", GetUserProfileKey(user), profile);
  if (ret == RedisCodeError) {
    LOG_ERROR << "get user profile failed! user: " << user; 
    return kCgiCodeSystemError;
  } else if (ret == RedisCodeNil) {
    LOG_ERROR << "please set user profile first! user: " << user; 
    return kCgiCodeUserProfileInexist;
  }

  return kCgiCodeOk;
}

int ProfileApi::Set(const std::string &user, const UserProfile &profile) {
  RedisStr2Pb<UserProfile> redis_pb;
  RedisCode ret = redis_pb.Query("SET", GetUserProfileKey(user), profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "set user profile failed! user: " << user; 
    return kCgiCodeSystemError;
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

int ProfileApi::MGet(const std::set<std::string> &users,
    std::map<std::string, UserProfile> *profiles) {
  std::vector<std::string> keys;
  for (std::set<std::string>::const_iterator iter = users.begin();
      iter != users.end(); ++iter) {
    keys.push_back(GetUserProfileKey(*iter));
  }

  RedisStr2Pb<UserProfile> redis_pb;
  std::map<std::string, UserProfile> tmp;
  if (redis_pb.Query("MGET", keys, &tmp) != RedisCodeOK) {
    return kCgiCodeSystemError;
  }

  for (std::map<std::string, UserProfile>::iterator iter = tmp.begin();
      iter != tmp.end(); ++iter) {
    (*profiles)[GetUserFromProfileKey(iter->first)] = iter->second;
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

void ProfileApi::Profile2Base(const UserProfile &profile, StripUserProfile *base) {
  base->set_user(profile.user());
  base->set_nickname(profile.nickname());
  base->set_head(profile.head());
}
