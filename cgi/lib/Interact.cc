#include "cgi/lib/Interact.h"
#include <time.h>
#include "common/utils/string_utils.h"
// #include "cgi/lib/CacheKeys.h"
#include "common/app/RedisKeys.h"
#include "cgi/lib/CgiCode.h"
#include "thirdparty/plog/Log.h"
#include "proto/interact.pb.h"

message InteractRoughData {
  string id = 1;                      // 互动id
  string user = 2;                    // 用户
  int64 time = 3;                     // 互动时间
  InterType type = 4;                 // 互动类型
  string comment = 5;                 // 互动内容,只对评论有效
  string record_id = 6;               // 互动记录id
  string target_interact_id = 7;      // 互动对象互动id
  bool is_delete = 8;                 // 是否删除
}
int Interact::Like(const LikeReq &req) {
  UserProfile profile;
  std::string key = kProfilePrefix + user_;
  RedisCode ret = profile_redis_.Query("GET", key, &profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user profile failed! user: " << user_; 
    return kCgiCodeSystemError;
  }

  InterData inter_data;
  inter_data.set_id(CreateInterId());
  inter_data.set_type(INTER_TYPE_LIKE);
  inter_data.set_time(time(NULL));
  UserBaseInfo *user_info = inter_data.mutable_user_info();
  user_info->set_user(user_);
  user_info->set_nickname(profile.nickname());
  user_info->set_head(profile.head_url());

  // set like data
  key = kRecordInteractPrefix + req.record_id();
  ret = inter_redis_.Query("HSET", key, user_, inter_data);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "set like data failed! user: "
      << user_ << "like " << req.record_id();
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int Interact::Unlike(const UnlikeReq &req) {
  std::string key = kRecordInteractPrefix + req.record_id();
  RedisCode ret = redis_.Query("HDEL", key, user_);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "unlike data failed! user: "
      << user_ << " record: " << req.record_id();
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int Interact::Comment(const CommentReq &req) {
  UserProfile profile;
  std::string key = kProfilePrefix + user_;
  RedisCode ret = profile_redis_.Query("GET", key, &profile);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get user profile failed! user: " << user_; 
    return kCgiCodeSystemError;
  }

  InterData comment;
  comment.set_id(CreateInterId());
  comment.set_type(INTER_TYPE_COMMENT);
  comment.set_time(time(NULL));
  comment.set_comment(req.comment());
  if (!req.comment_id().empty()) {
    key = kRecordInteractPrefix + req.record_id();
    InterData target_comment;
    ret = inter_redis_.Query("HGET", key, req.comment_id(), &target_comment);
    if (ret != RedisCodeOK) {
      LOG_ERROR << "get target comment info failed! record: "
        << req.record_id() << ", comment: " << req.comment_id(); 
      return kCgiCodeSystemError;
    }

    if (target_comment.user_info().nickname().empty()) {
      comment.set_target_nickname(target_comment.user_info().user());
    } else {
      comment.set_target_nickname(target_comment.user_info().nickname());
    }
  }

  UserBaseInfo *user_info = comment.mutable_user_info();
  user_info->set_user(user_);
  user_info->set_nickname(profile.nickname());
  user_info->set_head(profile.head_url());

  key = kRecordInteractPrefix + req.record_id();
  ret = inter_redis_.Query("HSET", key, req.comment_id(), comment);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "add comment failed! record: " << req.record_id()
      << ", comment: " << comment.id();
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int Interact::Uncomment(const UncommentReq &req) {
  std::string key = kRecordInteractPrefix + req.record_id();
  RedisCode ret = redis_.Query("HDEL", key, req.comment_id());
  if (ret != RedisCodeOK) {
    LOG_ERROR << "uncomment failed! user: " << user_ <<
      " record: " << req.record_id() << " comment: " << req.comment_id();
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int Interact::InteractDetail(const InteractDetailReq &req,
    InteractDetailRes *res) {
  std::string key = kRecordInteractPrefix + req.record_id();
  std::map<std::string, InterData> interacts;
  RedisCode ret = inter_redis_.Query("HGETALL", key, &interacts);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get record interacts failed! user: " << user_ <<
      " record: " << req.record_id();
    return kCgiCodeSystemError;
  }

  std::map<std::string, InterData>::const_iterator iter;
  for (iter = interacts.begin(); iter != interacts.end(); ++iter) {
    InterData *inter_data;
    if (iter->second.type() == INTER_TYPE_LIKE) {
      inter_data = res->add_likes();
    } else {
      inter_data = res->add_comments();
    }

    *inter_data = iter->second;
  }


  return kCgiCodeOk;
}

int Interact::InteractNotice(const InteractListReq &req,
    InteractListRes *res) {
  std::string key = kInteractNoticePrefix + user_;
  return GetInteracts(key, res);
}

int Interact::InteractHistory(const InteractListReq &req,
    InteractListRes *res) {
  std::string key = kInteractHistoryPrefix + user_;
  return GetInteracts(key, res);
}

int Interact::GetInteracts(const std::string &key, InteractListRes *res) {
  RedisStr2Pb<InterElement> inter_element_redis_;
  std::vector<InterElement> interacts;
  RedisCode ret = inter_element_redis_.Query("LRANGE", key, 0, -1, &interacts);
  if (ret != RedisCodeOK) {
    LOG_ERROR << "get interact list failed! key: " << key; 
    return kCgiCodeSystemError;
  }

  std::vector<InterElement>::const_iterator iter;
  for (iter = interacts.begin(); iter != interacts.end(); ++iter) {
    InterElement *inter_element = res->add_interacts();
    *inter_element = *iter;
  }
  
  return kCgiCodeOk;
}

std::string Interact::CreateInterId() {
  return user_ + value_to_string(time(NULL));
}

