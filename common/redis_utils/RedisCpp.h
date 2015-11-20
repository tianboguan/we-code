#ifndef COMMON_REDIS_UTILS_REDISCPP_H_
#define COMMON_REDIS_UTILS_REDISCPP_H_

#include <string>
#include <sstream>
#include "thirdparty/hiredis-master/hiredis.h"
#include "common/redis_utils/RedisBase.h"

class RedisCpp : public RedisBase {
  public:
    // time_out ms 
    RedisCpp(int time_out = 500) : RedisBase(time_out) {};

    // cmd input 1 param, test cmd exec status 
    // such as: INCR key
    // return 0 on success, < 0 on error
    RedisCode Query(std::string cmd, const std::string &key) {
      if (Connect()) {
        return RedisCodeError;
      }

      std::string format = cmd + " %b ";
      redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
          key.data(), key.size());
      if (reply->type == REDIS_REPLY_ERROR) {
        err_oss << cmd << " " << key << ", info: " << reply->str;
        return RedisCodeError;
      }

      return RedisCodeOK;
    }

    // cmd input 2 params, test cmd exec status 
    // such as: SET key value
    // return 0 on success, < 0 on error
    RedisCode Query(std::string cmd, const std::string &key, const std::string &value) {
      if (Connect()) {
        return RedisCodeError;
      }

      std::string format = cmd + " %b %b ";
      redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
        key.data(), key.size(), value.data(), value.size());
      if (reply->type == REDIS_REPLY_ERROR) {
        err_oss << cmd << " " << key << " " << value
          << ", info: " << reply->str;
        return RedisCodeError;
      }

      return RedisCodeOK;
    }

    // cmd input 1 param, return a value
    // such as GET key
    // return 0 on success, < 0 on error
    RedisCode Query(std::string cmd, const std::string &key, std::string *value) {
      if (Connect()) {
        return RedisCodeError;
      }

      std::string format = cmd + " %b ";
      redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
        key.data(), key.size());
      if (reply->type == REDIS_REPLY_ERROR) {
        err_oss << cmd << " " << key << ", info: " << reply->str;
        return RedisCodeError;
      } else if (reply->type == REDIS_REPLY_NIL) {
        return RedisCodeNil;
      }

      value->assign(reply->str, reply->len);
      return RedisCodeOK;
    }
};

#endif // COMMON_REDIS_UTILS_REDISCPP_H_
