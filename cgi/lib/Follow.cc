#include "cgi/lib/Follow.h"
#include "cgi/lib/CacheKeys.h"
#include "cgi/lib/CgiCode.h"
#include "thirdparty/plog/Log.h"

int Follow::Add(const FollowReq &req) {
  std::string key(kFollowPrefix + user_) ;
  RedisCode ret;
  bool blocked = false;

  IsBlocked(req.target_user(), &blocked);
  if (blocked) {
    return kCgiCodeBlocked;
  }

  // 把目标用户添加自己的关注列表
  ret = redis_.Query("SADD", key, req.target_user());
  if (ret == RedisCodeError) {
    return kCgiCodeSystemError;
  }

  // 把自己加入目标用户的被关注列表
  key = kFollowedPrefix + req.target_user();
  ret = redis_.Query("SADD", key, user_);
  if (ret == RedisCodeError) {
    LOG_ERROR << "user " << user_ << "add himself to " << req.target_user()
      << "'s followed list falied!";
    return kCgiCodeSystemError ;
  }

  return 0;
}

int Follow::Del(const FollowReq &req) {
  std::string key(kFollowPrefix + user_) ;
  RedisCode ret;

  // 从自已的关注列表里删除
  ret = redis_.Query("SREM", key, req.target_user());
  if (ret == RedisCodeError) {
    LOG_ERROR << "user " << user_ << "del user " << req.target_user()
      << " from his followed list falied!";
    return kCgiCodeSystemError;
  }

  // 从对方的被关注列表里删除
  key = kFollowedPrefix + req.target_user();
  ret = redis_.Query("SREM", key, user_);
  if (ret == RedisCodeError) {
    LOG_ERROR << "user " << user_ << "del himself from " << req.target_user()
      << "'s followed list falied!"; 
    return kCgiCodeSystemError;
  }

  return 0;
}

int Follow::FollowList(const FollowReq &req, FollowListRes *res) {
  std::string key(kFollowPrefix + req.target_user()) ;
  RedisCode ret;
  std::vector<std::string> users;

  ret = redis_.Query("SMEMBERS", key, &users);
  if (ret == RedisCodeError) {
    LOG_ERROR << "get user " << req.target_user()
      << "'s follow list falied!";
    return kCgiCodeSystemError;
  }

  return BuildFollowListRes(users, kFollowPrefix, res);
}

int Follow::FollowedList(const FollowReq &req, FollowListRes *res) {
  std::string key(kFollowedPrefix + req.target_user()) ;
  RedisCode ret;
  std::vector<std::string> users;

  ret = redis_.Query("SMEMBERS", key, &users);
  if (ret == RedisCodeError) {
    LOG_ERROR << "get user " << req.target_user()
      << "'s follow list falied!"; 
    return kCgiCodeSystemError;
  }

  return BuildFollowListRes(users, kFollowedPrefix, res);
}

int Follow::FollowStatus(const FollowReq &req, FollowStatusRes *res) {
  int test = 0;
  std::string key(kFollowPrefix + user_);
  RedisCode ret;

  ret = redis_.Query("SISMEMBER", key, req.target_user(), &test);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user " << user_ << "follow " << req.target_user()
      << " status falied!"; 
    return kCgiCodeSystemError;
  }

  test == 0 ? res->set_follow(false) :res->set_follow(true);
  return 0;
}

int Follow::Block(const FollowReq &req) {
  std::string key(kBlockUserPrefix + user_) ;
  RedisCode ret;

  // 把目标用户添加自己的黑名单列表
  ret = redis_.Query("SADD", key, req.target_user());
  if (ret == RedisCodeError) {
    LOG_ERROR << "user " << user_ << "add user " << req.target_user()
      << " to his block list falied!"; 
    return kCgiCodeSystemError;
  }

  // 把自己从目标用户的关注列表删除
  key = kFollowPrefix + req.target_user();
  ret = redis_.Query("SREM", key, user_);
  if (ret == RedisCodeError) {
    LOG_ERROR << "user " << user_ << "delete himself from " << req.target_user()
      << "'s followed list falied!";
    return kCgiCodeSystemError;
  }

  return 0;
}

int Follow::DeBlock(const FollowReq &req) {
  std::string key(kBlockUserPrefix + user_) ;
  RedisCode ret;

  // 把目标用户从黑名单列表里删除
  ret = redis_.Query("SREM", key, req.target_user());
  if (ret == RedisCodeError) {
    LOG_ERROR << "user " << user_ << "del user " << req.target_user()
      << " from his block list falied!";
    return kCgiCodeSystemError;
  }

  return 0;
}

int Follow::IsBlocked(const std::string &target_user, bool *blocked) {
  int test = 0;
  std::string key(kBlockUserPrefix + user_);
  RedisCode ret = redis_.Query("SISMEMBER", key, target_user, &test);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user " << user_ << "block " << target_user
      << " status falied!";
    return kCgiCodeSystemError;
  }

  *blocked = (test == 0 ? false : true);
  return 0;
}

int Follow::BuildFollowListRes(const std::vector<std::string> &users,
    std::string key_prefix, FollowListRes *res) {
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

  return 0;
}