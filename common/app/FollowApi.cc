#include "common/app/FollowApi.h"
#include "common/app/RedisKeys.h"
#include "common/app/CgiCode.h"
#include "thirdparty/plog/Log.h"

int FollowApi::Follow(const std::string &target_user) {
  if (target_user == user_) {
    LOG_ERROR << "user can't follow himself, user: " << user_;
    return kCgiCodeFollowOperatorSelf;
  }

  // check block status
  bool blocked = false;
  if(IsBlocked(target_user, user_,  &blocked) != kCgiCodeOk) {
    LOG_ERROR << "add user " << user_ << " follow " << target_user << " failed";
    return kCgiCodeSystemError;
  } else if (blocked) {
    return kCgiCodeBlocked;
  }

  // add action
  FollowAction action;
  action.set_type(FOLLOW_TYPE_FOLLOW);
  action.set_time(time(NULL));
  action.set_user(user_);
  action.set_target_user(target_user);
  RedisCode ret = AddFollowAction(action);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "add user " << user_ << " follow " << target_user
      << " action failed";
    return kCgiCodeSystemError;
  }

  // add target_user to user follow to list
  RedisCpp redis;
  ret = redis.Query("SADD", GetFollowToKey(user_), target_user);
  if (ret == RedisCodeError) {
    LOG_ERROR << "add user " << user_ << " follow " << target_user <<
      " follow to list failed";
    return kCgiCodeSystemError;
  }

  // add user to target_user follow from list
  ret = redis.Query("SADD", GetFollowFromKey(target_user), user_);
  if (ret == RedisCodeError) {
    LOG_ERROR << "add user " << user_ << " follow " << target_user <<
      " follow from list failed";
    return kCgiCodeSystemError ;
  }

  return kCgiCodeOk;
}

int FollowApi::UnFollow(const std::string &target_user) {
  // add action
  FollowAction action;
  action.set_type(FOLLOW_TYPE_UNFOLLOW);
  action.set_time(time(NULL));
  action.set_user(user_);
  action.set_target_user(target_user);
  RedisCode ret = AddFollowAction(action);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "add user " << user_ << " unfollow" << target_user
      << " action failed";
    return kCgiCodeSystemError;
  }

  // del target_user from user follow to list 
  RedisCpp redis;
  ret = redis.Query("SREM", GetFollowToKey(user_), target_user);
  if (ret == RedisCodeError) {
    LOG_ERROR << "del user " << user_ << " follow " << target_user <<
      " follow to list failed";
    return kCgiCodeSystemError;
  }

  // del user from target_user follow from list
  ret = redis.Query("SREM", GetFollowFromKey(target_user), user_);
  if (ret == RedisCodeError) {
    LOG_ERROR << "del user " << user_ << " follow " << target_user <<
      " follow from list failed";
    return kCgiCodeSystemError;
  }

  return 0;
}

int FollowApi::GetFollowTo(const std::string &target_user,
        std::vector<std::string> *users) {
  RedisCpp redis;
  return redis.Query("SMEMBERS", GetFollowToKey(target_user), users)
    == RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int FollowApi::GetFollowFrom(const std::string &target_user,
        std::vector<std::string> *users) {
  RedisCpp redis;
  return redis.Query("SMEMBERS", GetFollowFromKey(target_user), users)
    == RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int FollowApi::IsFollowed(const std::string &target_user, bool *followed) {
  int test = 0;
  RedisCpp redis;
  RedisCode ret = redis.Query("SISMEMBER", GetFollowToKey(user_),
      target_user, &test);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user " << user_ << " follow " << target_user
      << " status falied!"; 
    return kCgiCodeSystemError;
  }

  *followed = (test == 0 ? false : true);
  return kCgiCodeOk;
}

int FollowApi::Block(const std::string &target_user) {
  if (target_user == user_) {
    LOG_ERROR << "user can't block himself, user: " << user_;
    return kCgiCodeFollowOperatorSelf;
  }
  // add action
  FollowAction action;
  action.set_type(FOLLOW_TYPE_BLOCK);
  action.set_time(time(NULL));
  action.set_user(user_);
  action.set_target_user(target_user);
  RedisCode ret = AddFollowAction(action);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "add user " << user_ << " block " << target_user
      << " action failed";
    return kCgiCodeSystemError;
  }

  // del target_user from user_ follow from list 
  RedisCpp redis;
  ret = redis.Query("SREM", GetFollowFromKey(user_), target_user);
  if (ret == RedisCodeError) {
    LOG_ERROR << "delete user " << target_user << " from " << user_
      << "'s follow from list failed!";
    return kCgiCodeSystemError;
  }

  // del user_ from target_user follow to list 
  ret = redis.Query("SREM", GetFollowToKey(target_user), user_);
  if (ret == RedisCodeError) {
    LOG_ERROR << "delete user " << user_ << " from " << target_user
      << "'s follow to list failed!";
    return kCgiCodeSystemError;
  }

  // add target_user to user_ block list 
  ret = redis.Query("SADD", GetFollowBlockKey(user_), target_user);
  if (ret == RedisCodeError) {
    LOG_ERROR << "add " << target_user << " to " << user_
      << " block list falied!"; 
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int FollowApi::UnBlock(const std::string &target_user) {
  // add action
  FollowAction action;
  action.set_type(FOLLOW_TYPE_UNBLOCK);
  action.set_time(time(NULL));
  action.set_user(user_);
  action.set_target_user(target_user);
  RedisCode ret = AddFollowAction(action);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "add user " << user_ << " block " << target_user
      << " action failed";
    return kCgiCodeSystemError;
  }

  RedisCpp redis;
  return redis.Query("SREM", GetFollowBlockKey(user_), target_user) == 
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int FollowApi::IsBlocked(const std::string &from, const std::string &to,
    bool *blocked) {
  int test = 0;
  RedisCpp redis;
  RedisCode ret = redis.Query("SISMEMBER", GetFollowBlockKey(from),
      to, &test);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user " << from << "block " << to << " status falied!";
    return kCgiCodeSystemError;
  }

  *blocked = (test == 0 ? false : true);
  return kCgiCodeOk;
}

RedisCode FollowApi::AddFollowAction(const FollowAction &follow_action) {
  RedisStr2Pb<FollowAction> redis;
  return redis.Query("LPUSH", GetFollowActionKey(user_), follow_action);
}
