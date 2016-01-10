#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/app/ProfileApi.h"
#include "common/app/CgiCode.h"
#include "thirdparty/gflags/gflags.h"
#include <iostream>
#include "common/redis_utils/RedisPb.h"
#include "common/app/RedisKeys.h"
#include "proto/profile.pb.h"
#include "common/redis_utils/RedisCpp.h"
#include "common/redis_utils/RedisPb.h"
#include "common/pbjson/pbjson.h"

using namespace std;

DEFINE_string(user, "", "user");
DEFINE_string(action, "", "get or del");
DEFINE_string(type, "", "base or not");
DEFINE_string(json, "", "yes or not");

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "./profile_tool.log");
  ::google::ParseCommandLineFlags(&argc, &argv, true);
  cout << "--------------input-------------" << endl;
  cout << "user: " << FLAGS_user << endl;
  cout << "action: " << FLAGS_action << endl;
  cout << "type: " << FLAGS_type << endl;
  cout << "json: " << FLAGS_json << endl;
  cout << "\n----------------result------------" << endl;

  ProfileApi Api;
  if (FLAGS_action == "get") {
    std::string result;
    if (FLAGS_type == "base") {
      StripUserProfile info;
      if (Api.Get(FLAGS_user, &info) != kCgiCodeOk) {
        cout << "error, user: " << FLAGS_user << endl;
      } else {
        if (FLAGS_json == "yes") {
          // PbJsonUtil<UserBaseInfo> utils;
          // utils.Pb2Json(info, &result);

          pbjson::pb2json(&info, result);
        } else {
          result = info.Utf8DebugString();
        }
      }
    } else {
      UserProfile profile;
      if (Api.Get(FLAGS_user, &profile) != kCgiCodeOk) {
        cout << "error, user: " << FLAGS_user << endl;
      } else {
        if (FLAGS_json == "yes") {
          // PbJsonUtil<UserProfile> utils;
          // utils.Pb2Json(profile, &result);
          pbjson::pb2json(&profile, result);
          cout << "name: " << profile.nickname() << endl;
        } else {
          result = profile.Utf8DebugString();
        }
      }
    }

    cout << result << endl; 

  } else if (FLAGS_action == "del") {
      if (Api.Del(FLAGS_user) != kCgiCodeOk) {
        cout << "error, user: " << FLAGS_user << endl;
      } else {
        cout << "error, user: " << FLAGS_user << endl;
      }
  } else {
    cout << "invalid action type, option: get | del" << endl;
  }

  return 0;
}
