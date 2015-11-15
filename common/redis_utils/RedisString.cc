#include <string>
#include "thirdparty/hiredis-master/hiredis.h"
#include "common/redis_utils/RedisString.h"

using namespace std;

int RedisString::Get(const string& key, string& value) {
  int ret;
  ret = Connect();
  if (ret != 0) {
    return -1;
  }

  redisReply *reply = NULL;
  reply = redisCommand(c_,"GET %b ", key.data(), key.size());
  if (reply->str == NULL) {
    err_oss << "GET failed! key: " << key << endl;
    freeReplyObject(reply);
    return -1;
  }
  value.assign(reply->str, reply->len);
  freeReplyObject(reply);
  redisFree(c_);

  return 0;
}

int RedisString::Set(const string& key, const string& value) {
  int ret;
  ret = Connect();
  if (ret != 0) {
    return -1;
  }

  redisReply *reply = NULL;
  reply = redisCommand(c_,"SET %b %b", key.data(), key.size(),
                       value.data(), value.size()); 
  if (string("OK") != reply->str) {
    err_oss << "SET failed! key: " << key << ", value: " << value
      << ", Error: " << reply->str << endl;
    freeReplyObject(reply);
    return -1;
  }
  freeReplyObject(reply);
  redisFree(c_);

  return 0;
}

string RedisString::Error() {
  return err_oss.str();
}

int RedisString::Connect() {
  // connect
  struct timeval timeout = {0, 500};      // 0.5 seconds
  c_ = redisConnectWithTimeout("10.66.89.11", 6379, timeout);
  if (c_ == NULL || c_->err) {
    if (c_) {
      err_oss.clear();
      err_oss << "Connect error: " << c_->errstr << endl;
      redisFree(c_);
    } else {
      err_oss << "Connection error: can't allocate redis context" << endl;
    }
    return -1;
  }

  // auth
  redisReply *reply = NULL;
  reply = redisCommand(c_, "AUTH 8b9fb2df-7603-4155-887b-f70c1ae12dca:patientsclub0755");
  if (string("OK") != reply->str) {
    err_oss << "auth failed! " << endl;
    freeReplyObject(reply);
    return -1;
  }
  freeReplyObject(reply);

  return 0;
}

