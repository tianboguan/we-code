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
