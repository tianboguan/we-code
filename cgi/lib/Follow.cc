#include "cgi/lib/Follow.h"
#include <vector>
#include "common/app/CgiCode.h"
#include "common/app/ProfileApi.h"
#include "thirdparty/plog/Log.h"

int Follow::Add(const FollowReq &req) {
  return follow_api_.Follow(req.target_user());
}

int Follow::Del(const FollowReq &req) {
  return follow_api_.UnFollow(req.target_user());
}

int Follow::FollowList(const FollowReq &req, FollowListRes *res) {
  std::vector<std::string> users;
  int ret = follow_api_.GetFollowTo(req.target_user(), &users);
  if (ret != kCgiCodeOk) {
    return ret;
  }

  return BuildFollowListRes(users, res);
}

int Follow::FollowedList(const FollowReq &req, FollowListRes *res) {
  std::vector<std::string> users;
  int ret = follow_api_.GetFollowFrom(req.target_user(), &users);
  if (ret != kCgiCodeOk) {
    return ret;
  }

  return BuildFollowListRes(users, res);
}

int Follow::FollowStatus(const FollowReq &req, FollowStatusRes *res) {
  bool followed;
  int ret = follow_api_.IsFollowed(req.target_user(), &followed);
  if (ret != kCgiCodeOk) {
    return ret;
  }
  followed ? res->set_follow("yes") : res->set_follow("no");
  return kCgiCodeOk;
}

int Follow::Block(const FollowReq &req) {
  return follow_api_.Block(req.target_user());
}

int Follow::DeBlock(const FollowReq &req) {
  return follow_api_.UnBlock(req.target_user());
}

#if 0
int Follow::IsBlocked(const std::string &target_user, bool *blocked) {
  return follow_api_.IsBlocked(target_user, blocked);
}
#endif

int Follow::BuildFollowListRes(const std::vector<std::string> &users, FollowListRes *res) {

  std::vector<StripUserProfile> bases;
  ProfileApi profile_api;
  int ret = profile_api.MGet(users, &bases);
  if (ret != kCgiCodeOk) {
    return ret;
  }

  for (std::vector<StripUserProfile>::const_iterator iter = bases.begin();
      iter != bases.end(); ++iter) {
    FollowNode* node = res->add_node();
    StripUserProfile *user = node->mutable_user();
    *user = *iter;

    bool follow_status;
    follow_api_.IsFollowed(iter->user(), &follow_status);
    node->set_follow(follow_status);
  }

  return 0;
}
