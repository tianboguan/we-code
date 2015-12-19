#include "common/redis_utils/RedisCpp.h"
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "common/utils/StringUtils.h"
#include "thirdparty/plog/Log.h"
#include "thirdparty/hiredis-master/hiredis.h"

// cmd input 1 param, test cmd exec status 
// such as: INCR key
// return RedisCodeOK on sucess, other on error
RedisCode RedisCpp::Query(std::string cmd, const std::string &key) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << ", info: " << reply->str;
    freeReplyObject(reply);
    return RedisCodeError;
  }

  freeReplyObject(reply);
  return RedisCodeOK;
}

// cmd input 1 param, return a int value
// such as: EXISTS key
// return RedisCodeOK on sucess, other on error
RedisCode RedisCpp::Query(std::string cmd, const std::string &key, int64_t *value) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << ", info: " << reply->str;
    freeReplyObject(reply);
    return RedisCodeError;
  }

  *value = int64_t(reply->integer);
  freeReplyObject(reply);
  return RedisCodeOK;
}

// cmd input 2 params, test cmd exec status 
// such as: SET key value
// return RedisCodeOK on sucess, other on error
RedisCode RedisCpp::Query(std::string cmd, const std::string &key, const std::string &value) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b %b ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size(), value.data(), value.size());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << " " << value
      << ", info: " << reply->str;
    freeReplyObject(reply);
    return RedisCodeError;
  }

  freeReplyObject(reply);
  return RedisCodeOK;
}

// cmd input 3 param, test cmd exec status
// such as HSET key field field_value
// return RedisCodeOK on success, other on error
RedisCode RedisCpp::Query(std::string cmd, const std::string &key, 
    const std::string &param1, const std::string &param2) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b %b %b ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size(),
      param1.data(), param1.size(),
      param2.data(), param2.size());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << ", info: " << reply->str;
    freeReplyObject(reply);
    return RedisCodeError;
  } 

  freeReplyObject(reply);
  return RedisCodeOK;
}

// cmd input 2 param, return a int value
// such as SISMEMBER key member 
// return RedisCodeOK on sucess and set *value,  other on error
RedisCode RedisCpp::Query(std::string cmd, const std::string &key,
    const std::string &value, int *integer) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b %b ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size(), value.data(), value.size());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << " " << value << ", info: " << reply->str;
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
RedisCode RedisCpp::Query(std::string cmd, const std::string &key, std::string *value) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << ", info: " << reply->str;
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
// cmd input 2 param, return a string value
// such as HGET key
// return RedisCodeOK on sucess, RedisCodeNil on key not found, other on error
RedisCode RedisCpp::Query(std::string cmd, const std::string &key,
    const std::string &param1, std::string *value) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b %b ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size(), param1.data(), param1.size());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << ", info: " << reply->str;
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

// cmd input 1 param, return a vector of values
// such as SMEMBERS key
// return RedisCodeOK on success, other on error
RedisCode RedisCpp::Query(std::string cmd, const std::string &key,
    std::vector<std::string> *values) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << ", info: " << reply->str;
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

// cmd input 1 param, return a map of data 
// such as HGETALL key
// return RedisCodeOK on success, other on error
RedisCode RedisCpp::Query(std::string cmd, const std::string &key,
    std::map<std::string, std::string> *values) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << ", info: " << reply->str;
    freeReplyObject(reply);
    return RedisCodeError;
  } 

  if (reply->type == REDIS_REPLY_ARRAY) {
    if (reply->elements % 2 != 0) {
      LOG_ERROR << cmd << " " << key << ", return odd elements";
      freeReplyObject(reply);
      return RedisCodeError;
    }
    for (uint64_t i = 0; i < reply->elements; i += 2) {
      std::string k, v;
      k.assign(reply->element[i]->str, reply->element[i]->len);
      v.assign(reply->element[i+1]->str, reply->element[i+1]->len);
      (*values)[k] = v;
    }
  }
  freeReplyObject(reply);
  return RedisCodeOK;
}

// cmd input a set of params, return values with map
// such as MGET key1 key2 key3 ...
// return RedisCodeOK on success, other on error
// if a key return NIL then this key no contaned in map
RedisCode RedisCpp::Query(std::string cmd, const std::vector<std::string> &keys,
    std::map<std::string, std::string> *values) {
  if (keys.empty()) {
    return RedisCodeOK;
  }

  if (Connect()) {
    return RedisCodeError;
  }

  int token_count = keys.size() + 1;      // cmd + params

  char **argv = new char*[token_count];
  size_t *argv_len = new size_t[token_count];

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
    memcpy(argv[j], keys[i].data(), size);
    j++;
  }

  redisReply *reply = (redisReply *)redisCommandArgv(c_, keys.size() + 1,
      const_cast<const char**>(argv), argv_len);
  // ------------ free memory start ---------------
  for (int i = 0;  i < token_count; i++) {
    delete [] argv[i];
  }
  delete [] argv;
  delete [] argv_len;
  // ------------ free memory end ------------------

  if (reply == NULL) {
    LOG_ERROR << cmd << " invoke redisCommandArgv(...) return NULL";
    return RedisCodeError;
  } else if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " invoke redisCommandArgv(...), err: " << reply->str;
    freeReplyObject(reply);
    return RedisCodeError;
  } 

  if (reply->type == REDIS_REPLY_ARRAY) {
    if (reply->elements != keys.size()) {
      LOG_ERROR << cmd << " return value not match keys count";
      freeReplyObject(reply);
      return RedisCodeError;
    }

    for (uint64_t i = 0; i < reply->elements; i++) {
      if (reply->element[i]->type == REDIS_REPLY_NIL) {
        continue;
      }
      std::string temp;
      temp.assign(reply->element[i]->str, reply->element[i]->len);
      // set to empty string when return NIL
      (*values)[keys[i]] = temp;
    }
  }
  freeReplyObject(reply);
  return RedisCodeOK;
}

// cmd input a key and a range, return a set of values
// such as LRANGE key start stop
// return RedisCodeOK on success, other on error
RedisCode RedisCpp::Query(std::string cmd, const std::string &key, int start,
    int stop, std::vector<std::string> *values) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b %s %s ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size(), value_to_string(start).c_str(),
      value_to_string(stop).c_str());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << " " << start
      << " " << stop << ", info: " << reply->str;
    freeReplyObject(reply);
    return RedisCodeError;
  } 

  if (reply->type == REDIS_REPLY_ARRAY) {
    for (uint64_t i = 0; i < reply->elements; i++) {
      std::string v;
      v.assign(reply->element[i]->str, reply->element[i]->len);
      values->push_back(v);
    }
    freeReplyObject(reply);
    return RedisCodeOK;
  } else {
    LOG_ERROR << cmd << " " << key << " " << start
      << " " << stop << ", return not a array!";
    freeReplyObject(reply);
    return RedisCodeError;
  }
}

RedisCode RedisCpp::Query(std::string cmd, const std::string &key, int start,
        int stop) {
  if (Connect()) {
    return RedisCodeError;
  }

  std::string format = cmd + " %b %s %s ";
  redisReply *reply = (redisReply *)redisCommand(c_, format.c_str(),
      key.data(), key.size(), value_to_string(start).c_str(),
      value_to_string(stop).c_str());
  if (reply->type == REDIS_REPLY_ERROR) {
    LOG_ERROR << cmd << " " << key << " "<< start << " " << stop << ", info: "
      << reply->str;
    freeReplyObject(reply);
    return RedisCodeError;
  }

  freeReplyObject(reply);
  return RedisCodeOK;
}
