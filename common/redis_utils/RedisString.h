#ifndef COMMON_REDIS_UTILS_REDIS_STRING_H_
#define COMMON_REDIS_UTILS_REDIS_STRING_H_

#include <string>
#include <sstream>
#include "thirdparty/hiredis-master/hiredis.h"


using namespace std;

class RedisString {
  public:
    RedisString() {c_ = NULL; err_oss.clear();}
    int Get(const string& key, string& value);
    int Set(const string& key, const string& value);
    string Error();

  private:
    int Connect();

  private:
    redisContext *c_;
    ostringstream err_oss;
};

#endif // COMMON_REDIS_UTILS_REDIS_STRING_H_
