#include "cgi/lib/Interact.h"
#include <vector>
// #include <time.h>
// #include "common/utils/string_utils.h"
// #include "cgi/lib/CacheKeys.h"
#include "common/app/CgiCode.h"
#include "thirdparty/plog/Log.h"
//#include "proto/interact.pb.h"


int Interact::Like(const LikeReq &req) {
  return api_.Like(req.record_id());
}
int Interact::Unlike(const UnlikeReq &req) {
  return api_.Unlike(req.record_id());
}
int Interact::Comment(const CommentReq &req) {
  return api_.Comment(req.record_id(), req.text(), req.target_interact_id());
}
int Interact::Uncomment(const UncommentReq &req) {
  return api_.Uncomment(req.interact_id(), req.record_id());
}
int Interact::GetRecordInteracts(const RecordInteractReq &req,
    RecordInteractRes *res) {
  std::vector<ExtInteract> interacts;
  if (api_.GetRecordInteracts(req.record_id(), &interacts) != kCgiCodeOk) {
    LOG_ERROR << "get record interacts failed! record_id: " << req.record_id();
    return kCgiCodeSystemError;
  }

  std::vector<ExtInteract>::iterator iter;
  for (iter = interacts.begin(); iter != interacts.end(); ++iter) {
    if (iter->interact().type() == INTER_TYPE_LIKE) {
      ExtInteract *p = res->add_likes();
      *p = *iter;
    } else {
      ExtInteract *p = res->add_comments();
      *p = *iter;
    }
  }

  return kCgiCodeOk;
}

int Interact::GetNoticeInteracts(const UserInteractReq &req,
    UserInteractRes *res) {
  std::vector<ExtInteract> interacts;
  int ret = api_.GetUserNoticeInteracts(req.page(), &interacts);
  BuildUserInteracts(interacts, req.page(), res);
  return ret;
}
int Interact::GetSendedInteracts(const UserInteractReq &req,
    UserInteractRes *res) {
  std::vector<ExtInteract> interacts;
  int ret = api_.GetUserSendedInteracts(req.page(), &interacts);
  BuildUserInteracts(interacts, req.page(), res);
  return ret;
}
int Interact::GetReceivedInteracts(const UserInteractReq &req,
    UserInteractRes *res) {
  std::vector<ExtInteract> interacts;
  int ret = api_.GetUserReceivedInteracts(req.page(), &interacts);
  BuildUserInteracts(interacts, req.page(), res);
  return ret;
}

void Interact::BuildUserInteracts(const std::vector<ExtInteract> &interacts,
    int32_t page, UserInteractRes *res) {
  res->set_page(page);
  std::vector<ExtInteract>::const_iterator iter;
  for (iter = interacts.begin(); iter != interacts.end(); ++iter) {
    ExtInteract *p = res->add_interacts();
    *p = *iter;
  }
  return;
}
