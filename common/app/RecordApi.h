#ifndef COMMON_APP_RECORDAPI_H_
#define COMMON_APP_RECORDAPI_H_

#include <string>
#include <vector>
#include "proto/record.pb.h"

class RecordApi {
  public:
    RecordApi(const std::string &user) : user_(user), page_count_(12) {}; 

    int Get(const std::string &id, Record *record);
    int Get(const std::string &id, StripRecord *record);

    int GetRecordOwner(const std::string &id, std::string *user);

  private:
    std::string user_;
    int32_t page_count_;
    // RedisCpp redis_;
    // RedisStr2Pb<UserProfile> profile_redis_;
    // RedisStr2Pb<InterData> inter_redis_;
};

#endif // COMMON_APP_RECORDAPI_H_
