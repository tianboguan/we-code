#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/app/RecordApi.h"
#include "service/record/client/RecordClient.h"
#include "common/app/CgiCode.h"
#include "thirdparty/gflags/gflags.h"
#include "proto/profile.pb.h"
#include "common/utils/PbUtils.h"

using namespace std;

DEFINE_string(cmd, "get", "get | del | add");
DEFINE_string(user, "", "user id");
DEFINE_string(record, "", "record id");
DEFINE_string(profile, "./record.conf", "record profile file");

int main(int argc, char *argv[]) {
  ::google::SetUsageMessage("get,del or add record\n"
      "usage: record_tool <cmd> <args> \n\n"
      "for example:\n"
      "\tget user 123's record list run: \"record_tool -cmd=get -user=123\"\n"
      "\tget a record 1 info run: \"record_tool -cmd=get -record=1\"\n"
      "\tdel a record 1 run: \"record_tool -cmd=del -record=1\"\n"
      "\tadd a record run: \"record_tool -cmd=add -profile=./record.conf\"\n");
  plog::init(plog::debug, "./record_tool.log");
  ::google::ParseCommandLineFlags(&argc, &argv, true);

  RecordApi api(1024);
  int ret;
  if (FLAGS_cmd == "get") {
    if (FLAGS_user != "") {
      std::map<std::string, RoughRecord> records;
      ret = api.GetHomeRecord(FLAGS_user, 1, &records);
      if (ret == kCgiCodeSystemError) {
        cout << "get user " << FLAGS_user << "'s record list failed!" << endl;
      } else {
        std::map<std::string, RoughRecord>::iterator iter;
        for (iter = records.begin(); iter != records.end(); ++iter) {
          cout << "---------- record " << iter->first << " --------------" << endl;
          cout << iter->second.Utf8DebugString() << endl;
        }
      }
      return 0;
    } else if (FLAGS_record != "") {
      RoughRecord record;
      ret = api.Get(FLAGS_record, &record);
      if (ret != kCgiCodeOk) {
        cout << "get record " << FLAGS_record << " failed!" << endl;
      } else {
        cout << "get record success!" << endl;
        cout << record.Utf8DebugString() << endl;
      }
    }
    return 0;
  } else if (FLAGS_cmd == "del") {
    if (FLAGS_record != "") {
      ret = api.Del(FLAGS_record);
      if (ret != kCgiCodeOk) {
        cout << "del record " << FLAGS_record << " failed!" << endl;
      } else {
        cout << "del record " << FLAGS_record << " success!" << endl;
      }
    } else {
      cout << "missing record id to del" << endl; 
    }
    return 0;
  } else if (FLAGS_cmd == "add") {
    RoughRecord record;
    int ret = ParsePbConfigFromFile(FLAGS_profile, &record);
    if (ret != 0) {
      cout << "parse record info failed! file: " << FLAGS_profile << endl;
      return -1;
    }

    if (api.Set(record) != kCgiCodeOk) {
      cout << "Set record info failed!" << endl; 
      return -1;
    }

    RecordReq dispatch_req;
    dispatch_req.set_cmd(RECORD_CMD_ADD);
    dispatch_req.set_time(time(NULL));
    dispatch_req.set_id(record.id());
    RecordRes dispatch_res;
    RecordClient client;
    if (!(client.Send(dispatch_req, &dispatch_res))) {
      cout << "send record to dispatch server failed! error: " << client.Error() << endl;
    }
    return 0;
  } else {
    ::google::ShowUsageWithFlags(argv[0]);
  }
}
