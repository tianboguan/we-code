#ifndef CGI_LIB_RECORD_H_
#define CGI_LIB_RECORD_H_

#include <string>
#include "common/redis_utils/RedisCpp.h"
#include "common/redis_utils/RedisPb.h"
#include "proto/CgiReq.pb.h"
#include "proto/CacheData.pb.h"

class Record {
  public:
    Record (const std::string &user) : user_(user) {}; 

    int Create(const CreateRecordReq &req, CreateRecordRes *res);
    int Delete(const DelRecordReq &req);
    int QueryDetail(const QueryRecordDetailReq &req, QueryRecordDetailRes *res);
    int QueryList(const QueryRecordListReq &req, QueryRecordListRes *res);

  private:
    std::string CreateRecordId();

  private:
    std::string user_;
    RedisCpp redis_;
    RedisStr2Pb<UserProfile> profile_redis_;
    RedisStr2Pb<InterData> inter_redis_;
};

#endif // CGI_LIB_RECORD_H_
