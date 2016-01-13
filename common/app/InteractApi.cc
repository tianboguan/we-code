#include "common/app/InteractApi.h"
#include <time.h>
#include <string>
#include "common/utils/StringUtils.h"
#include "common/app/RedisKeys.h"
#include "common/app/CgiCode.h"
#include "common/app/ProfileApi.h"
#include "common/app/RecordApi.h"
#include "thirdparty/plog/Log.h"
#include "common/redis_utils/RedisPb.h"

// #include <iostream>

int InteractApi::Like(const std::string &record_id) {
  std::string id;
  int ret = CreateInterId(&id);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << "create like id failed! user: " << user_
      << " record:" << record_id; 
    return ret;
  }
  RoughInteract interact;
  interact.set_id(id);
  interact.set_user(user_);
  interact.set_time(time(NULL));
  interact.set_type(INTER_TYPE_LIKE);
  interact.set_text("");
  interact.set_record_id(record_id);
  interact.set_target_interact_id("");
  interact.set_is_delete(false);

  return DispatchInteract(interact);
}

int InteractApi::Unlike(const std::string &record_id) {
  std::vector<ExtInteract>  interacts;
  if (GetRecordInteracts(record_id, &interacts)) {
    LOG_ERROR << "read record interacts failed! record: " << record_id;
    return kCgiCodeSystemError;
  }
  std::string interact_id;
  std::vector<ExtInteract>::iterator iter;
  for (iter = interacts.begin(); iter != interacts.end(); ++iter) {
    if ((user_ == iter->user().user())
        && (iter->interact().type() == INTER_TYPE_LIKE)) {
      interact_id = iter->interact().id();
      break;
    }
  }
  if (iter == interacts.end()) {
    LOG_ERROR << "user: " << user_ << " try to unlike record: "
      << record_id << " but not like at all"; 
    return kCgiCodeOk;
  }

  return DelInteract(interact_id, record_id);
}

int InteractApi::Comment(const std::string &record_id, 
    const std::string &text,
    const std::string &target_interact_id) {
  std::string id;
  int ret = CreateInterId(&id);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << "create comment id failed! user: " << user_
      << " record:" << record_id; 
    return ret;
  }
  RoughInteract interact;
  interact.set_id(id);
  interact.set_user(user_);
  interact.set_time(time(NULL));
  interact.set_type(INTER_TYPE_COMMENT);
  interact.set_text(text);
  interact.set_record_id(record_id);
  interact.set_target_interact_id(target_interact_id);
  interact.set_is_delete(false);

  return DispatchInteract(interact);
}

int InteractApi::Uncomment(const std::string &comment_id,
    const std::string &record_id) {
  return DelInteract(comment_id, record_id);
}

int InteractApi::GetRecordInteracts(const std::string &record_id,
    std::vector<ExtInteract> *interacts) {
  std::vector<std::string> interact_ids;
  if (GetInteracts(GetRecordInteractDataKey(record_id), 0, -1,  &interact_ids)
      != kCgiCodeOk) {
    LOG_ERROR << "read record interact failed! record:" << record_id;
    return kCgiCodeSystemError;
  }

  std::vector<std::string> keys;
  for (std::vector<std::string>::iterator iter = interact_ids.begin(); 
      iter != interact_ids.end(); ++iter) {
    keys.push_back(GetInteractRecordDataKey(*iter));
  }

  if (GetInteracts(keys, interacts) != kCgiCodeOk) {
    LOG_ERROR << "read record interact detail list! record:" << record_id;
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int InteractApi::GetUserNoticeInteracts(int32_t page,
    std::vector<ExtInteract> *interacts) {
  std::vector<std::string> interact_ids;
  // page start begin 1 , range []
  int stop = page_count_ - 1;
  if (GetInteracts(GetUserNoticeInteractKey(user_), 0, stop,  &interact_ids)
      != kCgiCodeOk) {
    LOG_ERROR << "read user notice interact ids failed! user:" << user_;
    return kCgiCodeSystemError;
  }
  if (interact_ids.empty()) {
    return kCgiCodeNoMoreData;
  }

  std::vector<std::string> keys;
  for (std::vector<std::string>::iterator iter = interact_ids.begin(); 
      iter != interact_ids.end(); ++iter) {
    keys.push_back(GetInteractUserDataKey(*iter));
  }

  if (GetInteracts(keys, interacts) != kCgiCodeOk) {
    LOG_ERROR << "read record interact detail list! user:" << user_;
    return kCgiCodeSystemError;
  }

  // trim readed notice
  RedisCpp redis;
  if (redis.Query("LTRIM", GetUserNoticeInteractKey(user_), stop + 1, -1)
      != RedisCodeOK) {
    LOG_ERROR << "delete readed interact from notice failed! user:" << user_;
  }

  return kCgiCodeOk;
}

int InteractApi::GetUserSendedInteracts(int32_t page,
    std::vector<ExtInteract> *interacts) {
  int start = (page - 1) * page_count_;
  int stop = start + page_count_ - 1;
  std::vector<std::string> interact_ids;
  if (GetInteracts(GetUserSendedInteracKey(user_), start, stop, &interact_ids)
      != kCgiCodeOk) {
    LOG_ERROR << "read user history interact ids failed! user:" << user_;
    return kCgiCodeSystemError;
  }
  if (interact_ids.empty()) {
    return kCgiCodeNoMoreData;
  }

  std::vector<std::string> keys;
  for (std::vector<std::string>::iterator iter = interact_ids.begin(); 
      iter != interact_ids.end(); ++iter) {
    keys.push_back(GetInteractUserDataKey(*iter));
  }

  if (GetInteracts(keys, interacts) != kCgiCodeOk) {
    LOG_ERROR << "read record interact detail list! user:" << user_;
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}
int InteractApi::ClearRecordInteracts(const std::string &id) {
  RedisCpp redis;
  return redis.Query("DEL", GetRecordInteractDataKey(id)) == RedisCodeOK ?
    kCgiCodeOk : kCgiCodeSystemError;
}
int InteractApi::ClearUserNoticeInteracts() {
  RedisCpp redis;
  return redis.Query("DEL", GetUserNoticeInteractKey(user_)) == RedisCodeOK ?
    kCgiCodeOk : kCgiCodeSystemError;
}
int InteractApi::ClearUserReceivedInteracts() {
  RedisCpp redis;
  return redis.Query("DEL", GetUserReceivedInteractKey(user_)) == RedisCodeOK ?
    kCgiCodeOk : kCgiCodeSystemError;
}
int InteractApi::ClearUserSendedInteracts() {
  RedisCpp redis;
  return redis.Query("DEL", GetUserSendedInteracKey(user_)) == RedisCodeOK ? 
    kCgiCodeOk : kCgiCodeSystemError;
}

int InteractApi::GetUserReceivedInteracts(int32_t page,
    std::vector<ExtInteract> *interacts) {
  int start = (page - 1) * page_count_;
  int stop = start + page_count_ - 1;
  std::vector<std::string> interact_ids;
  if (GetInteracts(GetUserReceivedInteractKey(user_), start, stop,  &interact_ids)
      != kCgiCodeOk) {
    LOG_ERROR << "read user history interact ids failed! user:" << user_;
    return kCgiCodeSystemError;
  }

  if (interact_ids.empty()) {
    return kCgiCodeNoMoreData;
  }

  std::vector<std::string> keys;
  for (std::vector<std::string>::iterator iter = interact_ids.begin(); 
      iter != interact_ids.end(); ++iter) {
    keys.push_back(GetInteractUserDataKey(*iter));
  }

  if (GetInteracts(keys, interacts) != kCgiCodeOk) {
    LOG_ERROR << "read record interact detail list! user:" << user_;
    return kCgiCodeSystemError;
  }

  return kCgiCodeOk;
}

int InteractApi::CreateInterId(std::string *id) {
  RedisCpp redis;
  int64_t interact_id;
  if (redis.Query("INCR", GetInteractSequenceNoKey(), &interact_id)
      != RedisCodeOK) {
    return kCgiCodeSystemError;
  } else {
    *id = value_to_string(interact_id);
    return kCgiCodeOk;
  }
}

int InteractApi::DispatchInteract(const RoughInteract &interact) {
  if(SetRoughInteract(interact) != kCgiCodeOk) {
    LOG_ERROR << "write interact failed! info: " << interact.Utf8DebugString(); 
    return kCgiCodeSystemError;
  }

  ExtInteract ext_interact;
  if (Convert(interact, &ext_interact) != kCgiCodeOk) {
    LOG_ERROR << "convet interact from rough to ext failed!";
    return kCgiCodeSystemError;
  }

  // link to record
  if (ExtInteractUser(interact.user(), &ext_interact) != kCgiCodeOk) {
    LOG_ERROR << "ext interact user info failed! user: " << interact.user();
    return kCgiCodeSystemError;
  }
  if (SetExtRecordInteract(ext_interact) != kCgiCodeOk) {
    LOG_ERROR << "set ext interact user info failed! user: " << interact.user();
    return kCgiCodeSystemError;
  }
  if (LinkInteractToRecord(interact.id(), interact.record_id()) != kCgiCodeOk) {
    LOG_ERROR << "link interact: " << interact.id() << " to record "
      << interact.record_id() << " failed!";
    return kCgiCodeSystemError;
  }

  // link to user 
  if (ExtInteractRecord(interact.record_id(), &ext_interact) != kCgiCodeOk) {
    LOG_ERROR << "ext interact recourd info failed! interact: " << interact.id();
    return kCgiCodeSystemError;
  }
  if (SetExtUserInteract(ext_interact) != kCgiCodeOk) {
    LOG_ERROR << "set ext interact recourd info failed! interact: " << interact.id();
    return kCgiCodeSystemError;
  }
  if (LinkInteractToUserInteractTo(interact.id(), user_) != kCgiCodeOk) {
    LOG_ERROR << "link interact " << interact.id() << " to " << user_
      << "interact to list failed!";
  }
  std::string user;
  RecordApi api;
  if (api.GetRecordOwner(interact.record_id(), &user) != kCgiCodeOk) {
    LOG_ERROR << "get record owner failed! interact: " << interact.id() <<
      "record: " << interact.record_id();
    return kCgiCodeSystemError;
  }
  // interact self not need link to notice and interact from list
  if (user == user_) {
    return kCgiCodeOk;
  }
  if (LinkInteractToUserInteractFrom(interact.id(), user) != kCgiCodeOk) {
    LOG_ERROR << "link interact " << interact.id() << " to " << user
      << "interact from list failed!";
  }
  if (LinkInteractToUserInteractNotice(interact.id(), user) != kCgiCodeOk) {
    LOG_ERROR << "link interact " << interact.id() << " to " << user
      << "interact notice list failed!";
  }
  return kCgiCodeOk;
}

int InteractApi::DelInteract(const std::string &interact_id,
    const std::string &record_id) {
  UnlinkInteractFromRecord(interact_id, record_id);
  UnlinkInteractFromUserInteractTo(interact_id, user_);
  // DO NOT UNLINK Interact from;

  // Update interacct status to delete
  RoughInteract interact;
  if (GetRoughInteract(interact_id, &interact) != kCgiCodeOk) {
    LOG_ERROR << "in DelInteract() read interact rough data failed! "
      << "interact: " << interact_id;
  } else {
    interact.set_is_delete(true);
    SetRoughInteract(interact);
  }

  return kCgiCodeOk;
}

int InteractApi::GetInteracts(const std::string &key, int index_start,
    int index_stop, std::vector<std::string> *ids) {
  RedisCpp redis;
  if (redis.Query("LRANGE", key, index_start, index_stop, ids)
      == RedisCodeError) {
    LOG_ERROR << "read interact ids failed! key: " << key;
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}

int InteractApi::GetInteracts(const std::vector<std::string> &keys,
    std::vector<ExtInteract> *interacts) {
  RedisStr2Pb<ExtInteract> redis;
  std::map<std::string, ExtInteract> result;
  if (redis.Query("MGET", keys, &result) == RedisCodeError) {
    LOG_ERROR << "Mget interacts failed!"; 
    return kCgiCodeSystemError;
  }
  for (std::map<std::string, ExtInteract>::reverse_iterator iter = result.rbegin();
      iter != result.rend(); ++iter) {
    interacts->push_back(iter->second);
  }
  return kCgiCodeOk;
}

int InteractApi::Convert(const RoughInteract &from, ExtInteract *to){
  StripInteract *interact = to->mutable_interact();
  interact->set_id(from.id());
  interact->set_type(from.type());
  interact->set_time(from.time());
  interact->set_text(from.text());

  RoughInteract target;
  if (from.target_interact_id().empty()) {
    return kCgiCodeOk;
  } else {
    if (GetRoughInteract(from.target_interact_id(), &target) != kCgiCodeOk) {
      LOG_ERROR << "read interact failed! id: " << from.target_interact_id();
      return kCgiCodeSystemError;
    }
    UserProfile profile;
    RedisStr2Pb<UserProfile> profile_redis;
    if (profile_redis.Query("GET", GetUserProfileKey(target.user()), &profile)
        != kCgiCodeOk) {
      LOG_ERROR << "read user profile failed! user: " << target.user();
      return kCgiCodeSystemError;
    }

    interact->set_target_nickname(profile.nickname());
    return kCgiCodeOk;
  }
}

int InteractApi::ExtInteractUser(const std::string &user,
    ExtInteract *interact) {
  ProfileApi profile; 
  return profile.Get(user, interact->mutable_user());
}

int InteractApi::ExtInteractRecord(const std::string &record_id,
    ExtInteract *interact) {
  StripRecord record;
  RecordApi api;
  return api.Get(record_id, interact->mutable_record());
}

int InteractApi::SetRoughInteract(const RoughInteract &interact) {
  RedisStr2Pb<RoughInteract> redis;
  return redis.Query("SET", GetRoughInteractKey(interact.id()), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int InteractApi::GetRoughInteract(const std::string &id,
    RoughInteract *interact) {
  RedisStr2Pb<RoughInteract> redis;
  return redis.Query("GET", GetRoughInteractKey(id), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int InteractApi::SetExtUserInteract(const ExtInteract &interact) {
  RedisStr2Pb<ExtInteract> redis;
  return redis.Query("SET", GetInteractUserDataKey(interact.interact().id()), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int InteractApi::GetExtUserInteract(const std::string &id,
    ExtInteract *interact) {
  RedisStr2Pb<ExtInteract> redis;
  return redis.Query("GET", GetInteractUserDataKey(id), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int InteractApi::SetExtRecordInteract(const ExtInteract &interact) {
  RedisStr2Pb<ExtInteract> redis;
  return redis.Query("SET", GetInteractRecordDataKey(interact.interact().id()), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int InteractApi::GetExtRecordInteract(const std::string &id, ExtInteract *interact) {
  RedisStr2Pb<ExtInteract> redis;
  return redis.Query("GET", GetInteractRecordDataKey(id), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int InteractApi::LinkInteractToRecord(const std::string &interact,
    const std::string &record) {
  RedisCpp redis;
  return redis.Query("LPUSH", GetRecordInteractDataKey(record), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
int InteractApi::UnlinkInteractFromRecord(const std::string &interact,
    const std::string &record) {
  RedisCpp redis;
  return redis.Query("LREM", GetRecordInteractDataKey(record), "0", interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int InteractApi::LinkInteractToUserInteractTo(const std::string &interact, const std::string &user) {
  RedisCpp redis;
  return redis.Query("LPUSH", GetUserSendedInteracKey(user), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
int InteractApi::UnlinkInteractFromUserInteractTo(const std::string &interact, const std::string &user) {
  RedisCpp redis;
  return redis.Query("LREM", GetUserSendedInteracKey(user), "0", interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
int InteractApi::LinkInteractToUserInteractFrom(const std::string &interact, const std::string &user) {
  RedisCpp redis;
  return redis.Query("LPUSH", GetUserReceivedInteractKey(user), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
int InteractApi::UnlinkInteractFromUserInteractFrom(const std::string &interact, const std::string &user) {
  RedisCpp redis;
  return redis.Query("LREM", GetUserReceivedInteractKey(user), "0", interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
int InteractApi::LinkInteractToUserInteractNotice(const std::string &interact, const std::string &user) {
  RedisCpp redis;
  return redis.Query("LPUSH", GetUserNoticeInteractKey(user), interact) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
