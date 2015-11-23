#ifndef COMMON_REDIS_UTILS_REDISCPP_H_
#define COMMON_REDIS_UTILS_REDISCPP_H_

#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include "thirdparty/hiredis-master/hiredis.h"
#include "common/redis_utils/RedisBase.h"

class RedisCpp : public RedisBase {
  public:
    // time_out ms 
    RedisCpp(int time_out = 500) : RedisBase(time_out) {};

    // cmd input 1 param, test cmd exec status 
    // such as: INCR key
    // return RedisCodeOK on sucess, other on error
    RedisCode Query(std::string cmd, const std::string &key) {
      if (Connect()) {
        return RedisCodeError;
      }

      std::string format = cmd + " %b ";
      redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
          key.data(), key.size());
      if (reply->type == REDIS_REPLY_ERROR) {
        err_oss << cmd << " " << key << ", info: " << reply->str;
        freeReplyObject(reply);
        return RedisCodeError;
      }

      freeReplyObject(reply);
      return RedisCodeOK;
    }

    // cmd input 1 param, return a int value
    // such as: EXISTS key
    // return RedisCodeOK on sucess, other on error
    RedisCode Query(std::string cmd, const std::string &key, int *value) {
      if (Connect()) {
        return RedisCodeError;
      }

      std::string format = cmd + " %b ";
      redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
          key.data(), key.size());
      if (reply->type == REDIS_REPLY_ERROR) {
        err_oss << cmd << " " << key << ", info: " << reply->str;
        freeReplyObject(reply);
        return RedisCodeError;
      }

      *value = int(reply->integer);
      freeReplyObject(reply);
      return RedisCodeOK;
    }

    // cmd input 2 params, test cmd exec status 
    // such as: SET key value
    // return RedisCodeOK on sucess, other on error
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
        freeReplyObject(reply);
        return RedisCodeError;
      }

      freeReplyObject(reply);
      return RedisCodeOK;
    }

    // cmd input 2 param, return a int value
    // such as SISMEMBER key member 
    // return RedisCodeOK on sucess and set *value,  other on error
    RedisCode Query(std::string cmd, const std::string &key,
        const std::string &value, int *integer) {
      if (Connect()) {
        return RedisCodeError;
      }

      std::string format = cmd + " %b %b ";
      redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
          key.data(), key.size(), value.data(), value.size());
      if (reply->type == REDIS_REPLY_ERROR) {
        err_oss << cmd << " " << key << ", info: " << reply->str;
        freeReplyObject(reply);
        return RedisCodeError;
      } 

      *integer = int(reply->integer);
      freeReplyObject(reply);
      return RedisCodeError;
    }

    // cmd input 1 param, return a string value
    // such as GET key
    // return RedisCodeOK on sucess, RedisCodeNil on key not found, other on error
    RedisCode Query(std::string cmd, const std::string &key, std::string *value) {
      if (Connect()) {
        return RedisCodeError;
      }

      std::string format = cmd + " %b ";
      redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
          key.data(), key.size());
      if (reply->type == REDIS_REPLY_ERROR) {
        err_oss << cmd << " " << key << ", info: " << reply->str;
        freeReplyObject(reply);
        return RedisCodeError;
      } else if (reply->type == REDIS_REPLY_NIL) {
        freeReplyObject(reply);
        return RedisCodeNil;
      }

      value->assign(reply->str, reply->len);
      freeReplyObject(reply);
      return RedisCodeOK;
    }

    // cmd input 1 param, return a set of values
    // such as SMEMBERS key
    // return RedisCodeOK on success, other on error
    RedisCode Query(std::string cmd, const std::string &key,
        std::vector<std::string> *values) {
      if (Connect()) {
        return RedisCodeError;
      }

      std::string format = cmd + " %b ";
      redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
          key.data(), key.size());
      if (reply->type == REDIS_REPLY_ERROR) {
        err_oss << cmd << " " << key << ", info: " << reply->str;
        freeReplyObject(reply);
        return RedisCodeError;
      } 

      if (reply->type == REDIS_REPLY_ARRAY) {
        for (uint64_t i = 0; i < reply->elements; i++) {
          std::string temp;
          temp.assign(reply->element[i]->str, reply->element[i]->len);
          values->push_back(temp);
        }
      }
      freeReplyObject(reply);
      return RedisCodeOK;
    }

    // cmd input a set of params, return a set of values
    // such as MGET key1 key2 key3 ...
    // return RedisCodeOK on success, other on error
    RedisCode Query(std::string cmd, const std::vector<std::string> &keys,
        std::vector<std::string> *values) {
      if (keys.empty()) {
        return RedisCodeOK;
      }

      if (Connect()) {
        return RedisCodeError;
      }

      char **argv = new char*[keys.size() + 1];
      size_t *argv_len = new size_t[keys.size() + 1];

      // cmd
      argv[0] = new char[cmd.size()];
      memcpy(argv[0], cmd.data(), cmd.size());
      argv_len[0] = cmd.size();

      // keys
      int j = 1;
      for (size_t i = 0; i < keys.size(); i++) {
        size_t size = keys[i].size();
        argv_len[j] = size;
        argv[j] = new char[size];
        memcpy(argv[i], keys[i].data(), size);
        j++;
      }

      redisReply *reply = (redisReply *)redisCommandArgv(c_, keys.size() + 1,
          const_cast<const char**>(argv), argv_len);
      if (reply == NULL) {
        err_oss << cmd << " invoke redisCommandArgv(...) return NULL";
        return RedisCodeError;
      } else if (reply->type == REDIS_REPLY_ERROR) {
        err_oss << cmd << " invoke redisCommandArgv(...), err: " << reply->str;
        freeReplyObject(reply);
        return RedisCodeError;
      } 

      if (reply->type == REDIS_REPLY_ARRAY) {
        for (uint64_t i = 0; i < reply->elements; i++) {
          if (reply->element[i]->type == REDIS_REPLY_NIL) {
            continue;
          }
          std::string temp;
          temp.assign(reply->element[i]->str, reply->element[i]->len);
          values->push_back(temp);
        }
      }
      freeReplyObject(reply);

      // free memory
      for (size_t i = 0;  i < keys.size() + 1; i++) {
        delete [] argv[i];
      }
      delete [] argv;
      delete [] argv_len;

      return RedisCodeOK;
    }
};

#endif // COMMON_REDIS_UTILS_REDISCPP_H_
