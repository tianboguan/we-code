#include "service/record/src/worker.h"
#include <unistd.h>
#include <set>
#include "thirdparty/glog/logging.h"
#include "common/app/CgiCode.h"
#include "common/app/FollowApi.h"
// #include <iostream>

using namespace std;

bool RecordWorker::Handle(const RecordReq &req) {
  switch (req.cmd()) {
    case RECORD_CMD_ADD:
      return Add(req.id()); 
      break;
    default:
      LOG(ERROR) << "got unexpect req! req:\n" << req.DebugString();
      break;
  }
  return true;
}

bool RecordWorker::Add(const std::string &id) {
  LOG(ERROR) << "start to process add record request, record: " << id;
  RoughRecord record;
  if (api_.Get(id, &record) != kCgiCodeOk) {
    LOG(ERROR) << "failed to get record info, record: " << id;
    return false;
  }

  LOG_IF(ERROR, !LinkToHome(record)) << "link record: " << id << " to user: "
    << record.user() << " home failed!";

  LOG_IF(ERROR, !LinkToActive(record)) << "link record: " << id
    << " to users active failed!";

  LOG_IF(ERROR, !LinkToRecent(record)) << "link record: " << id
    << " to recent update list failed!";
  return true;
}

bool RecordWorker::LinkToHome(const RoughRecord &record) {
  return api_.LinkRecordToUserHome(record.id(), record.user()) == kCgiCodeOk;
}

bool RecordWorker::LinkToActive(const RoughRecord &record) {
  FollowApi follow_(record.user()); 
  std::vector<std::string> users; 
  if (follow_.GetFollowFrom(record.user(), &users) != kCgiCodeOk) {
    LOG(ERROR) << "got user follow from failed! user:\n" << record.user();
    return false;
  }

  std::set<std::string> user_set;
  for (size_t i = 0; i < users.size(); i++) {
    user_set.insert(users[i]);
  }
  // LinkToSelf
  user_set.insert(record.user());

  std::string id = record.id();
  for (std::set<std::string>::iterator iter = user_set.begin();
      iter != user_set.end(); ++iter) {
    LOG_IF(ERROR, api_.LinkRecordToUserActive(id, *iter) != kCgiCodeOk)
      << "failed link record " << id << " to user " << *iter;
  }
  return true;
}

bool RecordWorker::LinkToRecent(const RoughRecord &record) {
  return (api_.LinkRecordToRecent(record.id()) == kCgiCodeOk);
}
