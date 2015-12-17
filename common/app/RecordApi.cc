#include "common/app/RecordApi.h"
#include <time.h>
#include <string>
#include "common/utils/StringUtils.h"
#include "common/app/RedisKeys.h"
#include "common/app/CgiCode.h"
#include "common/app/ProfileApi.h"
#include "thirdparty/plog/Log.h"
#include "common/redis_utils/RedisPb.h"

int RecordApi::Get(const std::string &id, Record *record) {
  RedisStr2Pb<Record> redis;
  if (redis.Query("GET", GetRecordRoughDataKey(id), record) != RedisCodeOK) {
    LOG_ERROR << "get record rough info failed! record: " << id ;
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}

int RecordApi::Get(const std::string &id, StripRecord *striped) {
  Record record;
  if (Get(id, &record) != kCgiCodeOk) {
    return kCgiCodeSystemError;
  }
  striped->set_id(record.id());
  striped->set_text(record.text());
  if (record.picture_size() > 0) {
    striped->set_picture(record.picture(0));
  }
  return kCgiCodeOk;
}

int RecordApi::GetRecordOwner(const std::string &id, std::string *user) {
  Record record;
  if (Get(id, &record) != kCgiCodeOk) {
    return kCgiCodeSystemError;
  } else {
    *user = record.user();
    return kCgiCodeOk;
  }
}

int RecordApi::Set(const Record &record) {
  RedisStr2Pb<Record> redis;
  if (redis.Query("SET", GetRecordRoughDataKey(record.id()), record)
      != RedisCodeOK) {
    LOG_ERROR << "set record rough info failed! record: " << record.id() ;
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}

int RecordApi::CreateRecordId(std::string *id) {
  RedisCpp redis;
  return (redis.Query("INCR", GetRecordSequenceNoKey(), id) == RedisCodeOK
      ? kCgiCodeOk : kCgiCodeSystemError);
}

int RecordApi::GetHomeRecord(const std::string &user, int32_t page,
    std::vector<ExtRecord> *records) {
  int start = (page - 1) * page_count_;
  int stop = start + page_count_ - 1;
  std::vector<std::string> record_ids;
  if (GetRecords(GetUserHomeRecordKey(user), start, stop, &record_ids)
      != kCgiCodeOk ) {
    LOG_ERROR << "read user home record ids failed! user:" << user;
    return kCgiCodeSystemError;
  }

  std::vector<std::string> keys;
  for (std::vector<std::string>::iterator iter = record_ids.begin(); 
      iter != record_ids.end(); ++iter) {
    keys.push_back(GetRecordExtDataKey(*iter));
  }

  if (GetRecords(keys, records) != kCgiCodeOk) {
    LOG_ERROR << "read user home records failed! user:" << user;
    return kCgiCodeSystemError;
  }

  if (record_ids.size() < size_t(page_count_)) {
    return kCgiCodeNoMoreData;
  } else {
    return kCgiCodeMoreData;
  }

  return kCgiCodeOk;
}

int RecordApi::GetActiveRecord(const std::string &user, int32_t page,
    std::vector<ExtRecord> *records) {
  return kCgiCodeOk;
}

int RecordApi::GetRecentRecord(const std::string &user, int32_t page,
    std::vector<ExtRecord> *records) {
  return kCgiCodeOk;
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
  return redis.Query("LREM", GetUserHomeRecordKey(user), id) ==
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
  return redis.Query("LREM", GetUserActiveRecordKey(user), id) ==
    RedisCodeOK ? kCgiCodeOk : kCgiCodeSystemError;
}

int RecordApi::GetRecords(const std::string &key, int index_start,
    int index_stop, std::vector<std::string> *ids) {
  RedisCpp redis;
  if (redis.Query("LRANGE", key, index_start, index_stop, ids)
      == RedisCodeError) {
    LOG_ERROR << "read record ids failed! key: " << key;
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}
int RecordApi::GetRecords(const std::vector<std::string> &keys,
    std::vector<ExtRecord> *records) {
  RedisStr2Pb<ExtRecord> redis;
  if (redis.Query("MGET", keys, records) == RedisCodeError) {
    LOG_ERROR << "Mget interacts failed!"; 
    return kCgiCodeSystemError;
  }
  return kCgiCodeOk;
}
