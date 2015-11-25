#ifndef COMMON_REDIS_UTILS_REDISBASE_H_
#define COMMON_REDIS_UTILS_REDISBASE_H_

#include <string>
#include "thirdparty/hiredis-master/hiredis.h"

enum RedisCode {
  RedisCodeOK = 0,                               // 正常执行
  RedisCodeError = 1,                            // 执行错误
  RedisCodeNil = 2,                              // 无数据返回
};

class RedisBase{
  public:
    // time_out ms 
    RedisBase(int time_out) : c_(NULL), time_out_(time_out) {}

    ~RedisBase() {
      if (c_) {
        redisFree(c_);
      }
    }

    // connect redis server
    int Connect();

  public:
    redisContext *c_;

  private:
    int time_out_;            // communicate timeout 
};

#endif // COMMON_REDIS_UTILS_REDISBASE_H_
