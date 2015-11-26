#ifndef COMMON_REDIS_UTILS_REDISPB_H_
#define COMMON_REDIS_UTILS_REDISPB_H_

#include <string>
#include <sstream>
#include "common/redis_utils/RedisCpp.h"
//#include "common/utils/Pb2Json.h"

// key 为 string, value 为 pb
template<class V>
class RedisStr2Pb: public RedisCpp{
  public:
    RedisStr2Pb(int time_out = 200) : RedisCpp(time_out) {}

    // such as DECR 
    RedisCode Query(std::string cmd, const std::string &key) {
      return RedisCpp::Query(cmd, key);
    }

    RedisCode Query(std::string cmd, const std::string &key, const V &value) {
      std::string sv;
      value.SerializeToString(&sv);
      return RedisCpp::Query(cmd, key, sv);
    }

    // such as GET
    RedisCode Query(std::string cmd, const std::string &key, V *value) {
      std::string sv;
      RedisCode ret = RedisCpp::Query(cmd, key, &sv);
      if (ret != RedisCodeOK) {
        return ret;
      }

      value->ParseFromString(sv);
      return ret;
    }

    // such as MGET
    RedisCode Query(std::string cmd, const std::vector<std::string> &keys,
        std::vector<V> *values) {
      std::vector<std::string> vs;
      RedisCode ret = RedisCpp::Query(cmd, keys, &vs);
      if (ret != RedisCodeOK) {
        return ret;
      }
      std::vector<std::string>::const_iterator iter;
      for (iter = vs.begin(); iter != vs.end(); ++iter) {
        V value;
        value.ParseFromString(*iter);
        values->push_back(value);
      }
      return ret;
    }
};

// key, value均为pb结构
template<class K, class V>
class RedisPb2Pb: public RedisCpp{
  public:
    RedisPb2Pb(int time_out = 200) : RedisCpp(time_out) {}

    RedisCode Query(std::string cmd, const K &key) {
      std::string sk;
      key.SerializeToString(&sk);
      return RedisCpp::Query(cmd, sk);
    }

    RedisCode Query(std::string cmd, const K &key, const V &value) {
      std::string sk;
      key.SerializeToString(&sk);
      std::string sv;
      value.SerializeToString(&sv);
      return RedisCpp::Query(cmd, sk, sv);
    }

    RedisCode Query(std::string cmd, const K &key, V *value) {
      std::string sk;
      key.SerializeToString(&sk);
      std::string sv;
      RedisCode ret = RedisCpp::Query(cmd, sk, &sv);
      if (ret != RedisCodeOK) {
        return ret;
      }

      value->ParseFromString(sv);
      return ret;
    }
};

#endif // COMMON_REDIS_UTILS_REDISPB_H_
