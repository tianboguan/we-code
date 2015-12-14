#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "cgi/lib/Login.h"
#include "thirdparty/gflags/gflags.h"
#include <iostream>
#include "common/redis_utils/RedisPb.h"
#include "common/app/RedisKeys.h"
#include "common/redis_utils/RedisCpp.h"
#include "common/redis_utils/RedisPb.h"


using namespace std;
DEFINE_string(user, "", "user id");
DEFINE_string(phone, "", "user id");
DEFINE_string(action, "", "get or del or delkey");
DEFINE_string(key, "", "key for delkey action");

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "./account_tool.log");
  ::google::ParseCommandLineFlags(&argc, &argv, true);
  cout << "--------------input-------------" << endl;
  cout << "user: " << FLAGS_user << endl;
  cout << "phone: " << FLAGS_phone << endl;
  cout << "action: " << FLAGS_action << endl;
  cout << "\n----------------result------------" << endl;

  if (FLAGS_action == "get") {
    if (FLAGS_user != "") {
      string key = GetAccountUserKey(FLAGS_user);
      RedisStr2Pb<AccountInfo> redis;
      AccountInfo account;
      RedisCode ret = redis.Query("GET", key, &account);
      if (ret != RedisCodeOK) {
        cout << "get accoun info failed! key: " << key << endl;
      } else {
        cout << account.DebugString() << endl;
      }
    }

    if (FLAGS_phone!= "") {
      string key = GetAccountPhoneKey(FLAGS_phone);
      RedisCpp redis;
      string user;
      RedisCode ret = redis.Query("GET", key, &user);
      if (ret != RedisCodeOK) {
        cout << "get phone user info failed! key: " << key << endl;
      } else {
        cout << "user: " << user << endl;
        string key = GetAccountUserKey(user);
        RedisStr2Pb<AccountInfo> redis;
        AccountInfo account;
        RedisCode ret = redis.Query("GET", key, &account);
        if (ret != RedisCodeOK) {
          cout << "get accoun info failed! key: " << key << endl;
        } else {
          cout << account.DebugString() << endl;
        }
      }
    }
    return 0;
  } else if (FLAGS_action == "del") {
    if (FLAGS_phone!= "") {
      string key = GetAccountPhoneKey(FLAGS_phone);
      RedisCpp redis;
      RedisCode ret = redis.Query("DEL", key);
      if (ret != RedisCodeOK) {
        cout << "del phone info failed! key: " << key << endl;
      } else {
        cout << "del phone info success! key: " << key << endl;
      }
    }

    if (FLAGS_user!= "") {
      string key = GetAccountUserKey(FLAGS_user);
      RedisCpp redis;
      RedisCode ret = redis.Query("DEL", key);
      if (ret != RedisCodeOK) {
        cout << "del user info failed! key: " << key << endl;
      } else {
        cout << "del user info success! key: " << key << endl;
      }
    }

    return 0;
  } else if (FLAGS_action == "delkey") {
    RedisCpp redis;
    RedisCode ret = redis.Query("DEL", FLAGS_key);
    if (ret != RedisCodeOK) {
      cout << "del key failed! key: " << FLAGS_key<< endl;
    } else {
      cout << "del key success! key: " << FLAGS_key<< endl;
    }
  } else {
    cout << "invalid action type, option: get | del | delkey" << endl;
  }

  return 0;
}
