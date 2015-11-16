#ifndef COMMON_REDIS_UTILS_REDIS_PB_H_
#define COMMON_REDIS_UTILS_REDIS_PB_H_

#include <string>
#include <sstream>
#include "common/redis_utils/RedisString.h"

using namespace std;

// key, value均为pb结构
template<class K, class V>
class RedisPB2PB : public RedisString {
  public:
    RedisCode Get(const K& k, V& v) {
      string key;
      string value;
      k.SerializeToString(&key);
      RedisCode ret = RedisString::Get(key, value);
      if (ret != RedisCodeOK) {
        return ret;
      }
      v.ParseFromString(value);
      return RedisCodeOK;
    }

    RedisCode Set(const K& k, const V& v) {
      string key;
      string value;
      k.SerializeToString(&key);
      v.SerializeToString(&value);
      return RedisString::Set(key, value);
    }
};


// key为string, value为pb结构
template<class T>
class RedisStr2PB: public RedisString {
  public:
    RedisCode Get(const string& k, T& v) {
      string value;
      RedisCode ret = RedisString::Get(k, value);
      if (ret != RedisCodeOK) {
        return ret;
      }
      v.ParseFromString(value);
      return RedisCodeOK;
    }

    RedisCode Set(const string& k, const T& v) {
      string value;
      v.SerializeToString(&value);
      return RedisString::Set(k, value);
    }
};

#endif // COMMON_REDIS_UTILS_REDIS_PB_H_
