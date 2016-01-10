#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/app/ProfileApi.h"
#include "common/app/CgiCode.h"
#include "thirdparty/gflags/gflags.h"
#include "proto/profile.pb.h"
#include "common/utils/PbUtils.h"

using namespace std;

DEFINE_string(cmd, "get", "get or set");
DEFINE_string(user, "123", "user id");
DEFINE_string(profile, "./profile.conf", "profile config file");

int main(int argc, char *argv[]) {
  ::google::SetUsageMessage("set or get user's profile\n"
      "usage: profile_tool <cmd> <args> \n\n"
      "for example:\n"
      "\tget user 123's profile run: \"profile_tool -cmd=get -user=123\"\n"
      "\tset user profile run: \"profile_tool -cmd=set -profile=./profile.conf\", will parse user profile from \"./profile.conf\" and set to redis");
  plog::init(plog::debug, "./profile_tool.log");
  ::google::ParseCommandLineFlags(&argc, &argv, true);

  UserProfile profile;
  ProfileApi api;
  if (FLAGS_cmd == "get") {
    int ret = api.Get(FLAGS_user, &profile);
    if (ret != kCgiCodeOk) {
      cout << "get profile failed! user: " << FLAGS_user << " code: " << ret << ", msg: " << GetErrMsg(ret) << endl;
    } else {
      cout << "------------ User Profile------------\n" << profile.Utf8DebugString() << endl;
    }
    return 0;
  } else if (FLAGS_cmd == "set") {
    int ret = ParsePbConfigFromFile(FLAGS_profile, &profile);
    if (ret != 0) {
      cout << "parse profile failed! file: " << FLAGS_profile << endl; ;
      return -1;
    }

    ret = api.Set(profile.user(), profile);
    if (ret != kCgiCodeOk) {
      cout << "set profile failed! code: " << ret << ", msg: " << GetErrMsg(ret) << endl;
    } else {
      cout << "set user " << profile.user() << "success!";
    }
    return 0;
  } else {
    ::google::ShowUsageWithFlags(argv[0]);
  }
}
