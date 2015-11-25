
#include <string>
#include <sstream>
#include "common/redis_utils/RedisBase.h"
#include "thirdparty/plog/Log.h"


// connect redis server
int RedisBase::Connect() {
  struct timeval timeout; 
  timeout.tv_sec = time_out_ / 1000;
  timeout.tv_usec = time_out_ % 1000;

  c_ = redisConnectWithTimeout("10.66.89.11", 6379, timeout);
  if (c_->err != REDIS_OK) {
    LOG_ERROR << "Connect error: " << c_->errstr;
    redisFree(c_);
    return RedisCodeError;
  }

  // auth
  redisReply *reply = (redisReply *)redisCommand(c_,
      "AUTH 8b9fb2df-7603-4155-887b-f70c1ae12dca:patientsclub0755");
  if (REDIS_REPLY_STATUS != reply->type) {
    LOG_ERROR << "auth error: " << reply->str << ", type: " << reply->type;
    freeReplyObject(reply);
    return RedisCodeError;
  } else {
    freeReplyObject(reply);
    return RedisCodeOK;
  }
}

