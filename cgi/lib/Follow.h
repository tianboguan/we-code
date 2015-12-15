#ifndef CGI_LIB_FOLLOW_H_
#define CGI_LIB_FOLLOW_H_

#include <string>
#include "proto/CgiReq.pb.h"
#include "common/app/FollowApi.h"

class Follow {
  public:
    Follow(const std::string &user) : follow_api_(user) {}; 

    int Add(const FollowReq &req);
    int Del(const FollowReq &req);
    int FollowList(const FollowReq &req, FollowListRes *res);
    int FollowedList(const FollowReq &req, FollowListRes *res);
    int FollowStatus(const FollowReq &req, FollowStatusRes *res);
    int Block(const FollowReq &req);
    int DeBlock(const FollowReq &req);

  private:
    int BuildFollowListRes(const std::vector<std::string> &users,
        FollowListRes *res);

  private:
    FollowApi follow_api_;
};

#endif // CGI_LIB_FOLLOW_H_
