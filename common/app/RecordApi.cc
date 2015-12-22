#include "common/app/RecordApi.h"
#include <time.h>
#include <string>
#include "common/utils/StringUtils.h"
#include "common/app/RedisKeys.h"
#include "common/app/CgiCode.h"
#include "common/app/ProfileApi.h"
#include "thirdparty/plog/Log.h"
#include "common/redis_utils/RedisPb.h"
// #include <iostream>

int RecordApi::Get(const std::string &id, RoughRecord *record) {
  RedisStr2Pb<RoughRecord> redis;
  if (redis.Query("GET", GetRecordRoughDataKey(id), record) != RedisCodeOK) {
    LOG_ERROR << "get record rough info failed! record: " << id ;
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}

int RecordApi::Get(const std::string &id, StripRecord *striped) {
  RoughRecord record;
  if (Get(id, &record) != kCgiCodeOk) {
    return kCgiCodeSystemError;
  }
  striped->set_id(record.id());
  striped->set_text(record.text());
  if (record.pictures_size() > 0) {
    striped->set_picture(record.pictures(0));
  }
  return kCgiCodeOk;
}

int RecordApi::Del(const std::string &id) {
  RoughRecord record;
  int ret = Get(id, &record);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << "get record rough info failed! record: " << id ;
    return kCgiCodeSystemError;
  }
  record.set_is_delete(true);
  return Set(record);
}

int RecordApi::SetRecordPublic(const std::string &id, bool is_public) {
  RoughRecord record;
  int ret = Get(id, &record);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << "get record rough info failed! record: " << id ;
    return kCgiCodeSystemError;
  }
  record.set_is_public(is_public);
  return Set(record);
}

int RecordApi::GetRecordOwner(const std::string &id, std::string *user) {
  RoughRecord record;
  if (Get(id, &record) != kCgiCodeOk) {
    return kCgiCodeSystemError;
  } else {
    *user = record.user();
    return kCgiCodeOk;
  }
}

int RecordApi::Set(const RoughRecord &record) {
  RedisStr2Pb<RoughRecord> redis;
  if (redis.Query("SET", GetRecordRoughDataKey(record.id()), record)
      != RedisCodeOK) {
    LOG_ERROR << "set record rough info failed! record: " << record.id() ;
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}

int RecordApi::CreateRecordId(std::string *id) {
  RedisCpp redis;
  int64_t record_id;
  if (redis.Query("INCR", GetRecordSequenceNoKey(), &record_id) == RedisCodeOK) {
    *id = value_to_string(record_id);
    return kCgiCodeOk;
  } else {
    return kCgiCodeSystemError;
  }
}

int RecordApi::GetHomeRecord(const std::string &user, int32_t page,
    std::map<std::string, RoughRecord> *records) {
  int start = (page - 1) * page_count_;
  int stop = start + page_count_ - 1;
  std::vector<std::string> record_ids;
  if (GetRecords(GetUserHomeRecordKey(user), start, stop, &record_ids)
      != kCgiCodeOk ) {
    LOG_ERROR << "read user home record ids failed! user:" << user;
    return kCgiCodeSystemError;
  }

  if (GetRecords(record_ids, records) != kCgiCodeOk) {
    LOG_ERROR << "read user home records failed! user:" << user;
    return kCgiCodeSystemError;
  }

  if (record_ids.size() < size_t(page_count_)) {
    return kCgiCodeNoMoreData;
  } else {
    return kCgiCodeMoreData;
  }
}

int RecordApi::GetActiveRecord(const std::string &user, int32_t page,
    std::map<std::string, RoughRecord> *records) {
  int start = (page - 1) * page_count_;
  int stop = start + page_count_ - 1;
  std::vector<std::string> record_ids;
  if (GetRecords(GetUserActiveRecordKey(user), start, stop, &record_ids)
      != kCgiCodeOk ) {
    LOG_ERROR << "read user active record ids failed! user:" << user;
    return kCgiCodeSystemError;
  }

  if (GetRecords(record_ids, records) != kCgiCodeOk) {
    LOG_ERROR << "read user active records failed! user:" << user;
    return kCgiCodeSystemError;
  }

  if (record_ids.size() < size_t(page_count_)) {
    return kCgiCodeNoMoreData;
  } else {
    return kCgiCodeMoreData;
  }
}

int RecordApi::GetRecentRecord(const std::string &user, int32_t page,
    std::map<std::string, RoughRecord> *records) {
  int start = (page - 1) * page_count_;
  int stop = start + page_count_ - 1;
  std::vector<std::string> record_ids;
  // TODO get record list base user disease now get active key instead
  if (GetRecords(GetUserActiveRecordKey(user), start, stop, &record_ids)
      != kCgiCodeOk ) {
    LOG_ERROR << "read user active record ids failed! user:" << user;
    return kCgiCodeSystemError;
  }

  if (GetRecords(record_ids, records) != kCgiCodeOk) {
    LOG_ERROR << "read user active records failed! user:" << user;
    return kCgiCodeSystemError;
  }

  if (record_ids.size() < size_t(page_count_)) {
    return kCgiCodeNoMoreData;
  } else {
    return kCgiCodeMoreData;
  }
}

int RecordApi::LinkRecordToUserHome(const std::string &id,
    const std::string &user) {
  RedisCpp redis;
  return redis.Query("LPUSH", GetUserHomeRecordKey(user), id) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
int RecordApi::UnlinkRecordToUserHome(const std::string &id,
    const std::string &user) {
  RedisCpp redis;
  return redis.Query("LREM", GetUserHomeRecordKey(user), "0", id) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
int RecordApi::LinkRecordToUserActive(const std::string &id,
    const std::string &user) {
  RedisCpp redis;
  return redis.Query("LPUSH", GetUserActiveRecordKey(user), id) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
int RecordApi::UnlinkRecordToUserActive(const std::string &id,
    const std::string &user) {
  RedisCpp redis;
  return redis.Query("LREM", GetUserActiveRecordKey(user), "0", id) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}
int RecordApi::LinkRecordToRecent(const std::string &id) {
  RedisCpp redis;
  return redis.Query("LPUSH", GetRecordRecentKey(), id) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int RecordApi::GetRecords(const std::string &key, int index_start,
    int index_stop, std::vector<std::string> *ids) {
  RedisCpp redis;
  // std::cout << "start: " << index_start << " stop: " << index_stop << std::endl;
  if (redis.Query("LRANGE", key, index_start, index_stop, ids)
      == RedisCodeError) {
    LOG_ERROR << "read record ids failed! key: " << key;
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}

int RecordApi::GetRecords(const std::vector<std::string> &ids,
    std::map<std::string, RoughRecord> *records) {
  if (ids.empty()) {
    return kCgiCodeOk;
  }

  std::vector<std::string> keys;
  for (std::vector<std::string>::const_iterator iter = ids.begin(); 
      iter != ids.end(); ++iter) {
    keys.push_back(GetRecordRoughDataKey(*iter));
  }

  RedisStr2Pb<RoughRecord> redis;
  std::map<std::string, RoughRecord> tmp_records;
  if (redis.Query("MGET", keys, &tmp_records) == RedisCodeError) {
    LOG_ERROR << "Mget interacts failed!"; 
    return kCgiCodeSystemError;
  }

  for (std::map<std::string, RoughRecord>::iterator iter = tmp_records.begin();
      iter != tmp_records.end(); ++iter) {
    if (iter->second.is_delete()) {
      continue;
    }
    (*records)[(iter->second).id()] = iter->second;
  }
  
  return kCgiCodeOk;
}

