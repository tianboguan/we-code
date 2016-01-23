#include "common/app/StatisticApi.h"
#include <string>
#include <vector>
#include "thirdparty/plog/Log.h"
#include "common/redis_utils/RedisCpp.h"
#include "common/app/RedisKeys.h"
#include "common/app/CgiCode.h"

// record stat
int StatisticApi::ViewRecord(const std::string &id) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetRecordStatisticKey(id), "view", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "record " << id << " incr view stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::ViewRecords(const std::vector<std::string> &ids) {
  std::vector<std::string>::const_iterator iter;
  for (iter = ids.begin(); iter != ids.end(); ++iter) {
    ViewRecord(*iter);
  }
  return 0;
}

int StatisticApi::LikeRecord(const std::string &id) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetRecordStatisticKey(id), "like", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "record " << id << " incr like stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::UnlikeRecord(const std::string &id) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetRecordStatisticKey(id), "like", "-1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "record " << id << " decr like stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::CommentRecord(const std::string &id) { 
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetRecordStatisticKey(id), "comment", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "record " << id << " incr comment stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::UncommentRecord(const std::string &id) { 
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetRecordStatisticKey(id), "comment", "-1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "record " << id << " decr comment stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::GetRecordStat(const std::string &id, RecordStat *stat) {
  RedisCpp  redis;
  std::map<std::string, std::string> values;
  int ret = redis.Query("HGETALL", GetRecordStatisticKey(id), &values);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get record " << id << " statistic data failed!"; 
    return kCgiCodeSystemError;
  }
  stat->set_view(atoi(values["view"].c_str()));
  stat->set_like(atoi(values["like"].c_str()));
  stat->set_comment(atoi(values["comment"].c_str()));
  return kCgiCodeOk;
}

int StatisticApi::GetRecordsStat(const std::vector<std::string> &ids,
       std::map<std::string, RecordStat> *stats) {
  std::vector<std::string>::const_iterator iter;
  for (iter = ids.begin(); iter != ids.end(); ++iter) {
    GetRecordStat(*iter, &((*stats)[*iter]));
  }

  return kCgiCodeOk;
}

// user stat
int StatisticApi::AddRecord(const std::string &user) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetUserStatisticKey(user), "record", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "user " << user << " incr record stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::DelRecord(const std::string &user) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetUserStatisticKey(user), "record", "-1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "user " << user << " decr record stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::UserLike(const std::string &user) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetUserStatisticKey(user), "like", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "user " << user << " incr like stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::UserLiked(const std::string &user) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetUserStatisticKey(user), "liked", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "user " << user << " incr liked stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::UserComment(const std::string &user) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetUserStatisticKey(user), "comment", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "user " << user << " incr comment stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::UserCommented(const std::string &user) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetUserStatisticKey(user), "commented", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "user " << user << " incr commented stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::UserFollow(const std::string &user) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetUserStatisticKey(user), "follow", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "user " << user << " incr follow stat failed!"; 
    return -1;
  }
  return 0;
}

int StatisticApi::UserFollowed(const std::string &user) {
  RedisCpp  redis;
  int ret = redis.Query("HINCRBY", GetUserStatisticKey(user), "followed", "1");
  if (ret != RedisCodeOK) {
    LOG_ERROR << "user " << user << " incr followed stat failed!"; 
    return -1;
  }
  return 0;
  return 0;
}

int StatisticApi::GetUserStat(const std::string &user, UserStat *stat) {
  RedisCpp  redis;
  std::map<std::string, std::string> values;
  int ret = redis.Query("HGETALL", GetUserStatisticKey(user), &values);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user " << user << " statistic data failed!"; 
    return kCgiCodeSystemError;
  }
  stat->set_user(user);
  stat->set_login_count(atoi(values["login"].c_str()));
  stat->set_follow_count(atoi(values["follow"].c_str()));
  stat->set_followed_count(atoi(values["followed"].c_str()));
  stat->set_record_count(atoi(values["record"].c_str()));
  stat->set_comment_count(atoi(values["comment"].c_str()));
  stat->set_commented_count(atoi(values["commented"].c_str()));
  stat->set_like_count(atoi(values["like"].c_str()));
  stat->set_liked_count(atoi(values["liked"].c_str()));
  return kCgiCodeOk;
}

