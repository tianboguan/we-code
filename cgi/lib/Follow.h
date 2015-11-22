#ifndef CGI_LIB_FOLLOW_H_
#define CGI_LIB_FOLLOW_H_

#include <string>
#include <map>
#include <sstream>
#include "common/redis_utils/RedisPb.h"
#include "proto/CgiReq.pb.h"
#include "proto/CacheData.pb.h"

class Follow {
  public:
    Follow(std::string &user) : user_(user) {}; 

    int Add(const FollowReq &req);
    int Del(const FollowReq &req);
    int FollowList(const FollowReq &req, FollowListRes *res);
    int FollowedList(const FollowReq &req, FollowListRes *res);
    int FollowStatus(const FollowReq &req, FollowStatusRes *res);
    int Block(const FollowReq &req);
    int DeBlock(const FollowReq &req);
    int IsBlocked(const string &target_user, bool *blocked);

    string Error();

  private:
    int GetUserProfile(const std::vector<string> &users, FollowListRes *res);

  private:
    std::string user_;
    std::ostringstream err_oss_;
    RedisCpp redis_;
};

#endif // CGI_LIB_FOLLOW_H_
