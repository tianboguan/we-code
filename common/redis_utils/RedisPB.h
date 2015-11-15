#ifndef COMMON_REDIS_UTILS_REDIS_PB_H_
#define COMMON_REDIS_UTILS_REDIS_PB_H_

#include <string>
#include <sstream>
#include "common/redis_utils/RedisString.h"


using namespace std;
template<class K, class V>
class RedisPB : public RedisString {
  public:
    // key, value均为pb结构
    int Get(const K& k, V& v) {
      string key;
      string value;
      k.SerializeToString(&key);
      int ret = RedisString::Get(key, value);
      if (ret != 0) {
        return ret;
      }
      v.ParseFromString(value);
      return 0;
    }

    // key, value均为pb结构
    int Set(const K& k, const V& v) {
      string key;
      string value;
      k.SerializeToString(&key);
      v.SerializeToString(&value);
      return RedisString::Set(key, value);
    }

    // key为string, value为pb结构
    int Get(const string& k, V& v) {
      string value;
      int ret = RedisString::Get(k, value);
      if (ret != 0) {
        return ret;
      }
      v.ParseFromString(value);
      return 0;
    }

    // key为string, value为pb结构
    int Set(const string& k, const V& v) {
      string value;
      v.SerializeToString(&value);
      return RedisString::Set(k, value);
    }
    string Error() {
      return RedisString::Error();
    }
};

#endif // COMMON_REDIS_UTILS_REDIS_PB_H_
