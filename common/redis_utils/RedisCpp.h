#ifndef COMMON_REDIS_UTILS_REDISCPP_H_
#define COMMON_REDIS_UTILS_REDISCPP_H_

#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "common/redis_utils/RedisBase.h"

class RedisCpp : public RedisBase {
  public:
    // time_out ms 
    RedisCpp(int time_out = 500) : RedisBase(time_out) {};

    // cmd input 1 param, test cmd exec status 
    // such as: DEL key
    // return RedisCodeOK on sucess, other on error
    RedisCode Query(std::string cmd, const std::string &key);

    // cmd input 1 param, return a int value
    // such as: EXISTS key
    // return RedisCodeOK on sucess, other on error
    RedisCode Query(std::string cmd, const std::string &key, int64_t *value);

    // cmd input 2 params, test cmd exec status 
    // such as: SET key value
    // return RedisCodeOK on sucess, other on error
    RedisCode Query(std::string cmd, const std::string &key, const std::string &value);

    // cmd input 3 param, test cmd exec status
    // such as HSET/HINCRBY key field field_value
    // return RedisCodeOK on success, other on error
    RedisCode Query(std::string cmd, const std::string &key, 
        const std::string &param1, const std::string &param2);

    // cmd input 2 param, return a int value
    // such as SISMEMBER key member 
    // return RedisCodeOK on sucess and set *value,  other on error
    RedisCode Query(std::string cmd, const std::string &key,
        const std::string &value, int *integer);

    // cmd input 1 param, return a string value
    // such as GET key
    // return RedisCodeOK on sucess, RedisCodeNil on key not found, other on error
    RedisCode Query(std::string cmd, const std::string &key, std::string *value);

    // cmd input 2 param, return a string value
    // such as HGET key
    // return RedisCodeOK on sucess, RedisCodeNil on key not found, other on error
    RedisCode Query(std::string cmd, const std::string &key,
        const std::string &param1, std::string *value);

    // cmd input 1 param, return a vector of values
    // such as SMEMBERS key
    // return RedisCodeOK on success, other on error
    RedisCode Query(std::string cmd, const std::string &key,
        std::vector<std::string> *values);

    // cmd input 1 param, return a map of data 
    // such as HGETALL key
    // return RedisCodeOK on success, other on error
    RedisCode Query(std::string cmd, const std::string &key,
        std::map<std::string, std::string> *values);

    // cmd input a set of params, return a map of values
    // such as MGET key1 key2 key3 ...
    // return RedisCodeOK on success, other on error
    RedisCode Query(std::string cmd, const std::vector<std::string> &keys,
        std::map<std::string, std::string> *values);

    // cmd input a key and a range, return a set of values
    // such as LRANGE key start stop
    // return RedisCodeOK on success, other on error
    RedisCode Query(std::string cmd, const std::string &key, int start,
        int stop, std::vector<std::string> *values);

    // cmd input a key and a range, test cmd exec status 
    // such as LTRIM key start stop
    // return RedisCodeOK on success, other on error
    RedisCode Query(std::string cmd, const std::string &key, int start,
        int stop);
};

#endif // COMMON_REDIS_UTILS_REDISCPP_H_
