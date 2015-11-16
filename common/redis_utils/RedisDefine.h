#ifndef COMMON_REDIS_UTILS_REDIS_DEFINE_H_
#define COMMON_REDIS_UTILS_REDIS_DEFINE_H_

#include <string>
using namespace std;

const string kAcountPrefix = "account_";                // 帐号相关
const string kAppPrefix = "app_";                       // 业务管理相关
const string kInteractPrefix = "inter_";                // 互动相关
const string kFollowPrefix = "follow_";                 // 关系链相关
const string kProfilePrefix = "profile_";               // 个人资料相关
const string kPublicPrefix = "public_";                 // 发现页记录列表
const string kPrivatePrefix = "private_";               // 动态页及个人主页

enum RedisCode {
  RedisCodeOK = 0,                               // 正常执行
  RedisCodeError = 1,                            // 执行错误
  RedisCodeKeyNotFound = 2,                      // key不存在
};


#endif // COMMON_REDIS_UTILS_REDIS_DEFINE_H_
