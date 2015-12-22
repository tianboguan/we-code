#ifndef COMMON_APP_FOLLOWAPI_H_
#define COMMON_APP_FOLLOWAPI_H_

#include <string>
#include <vector>
#include "proto/CgiReq.pb.h"
#include "proto/follow.pb.h"
#include "common/redis_utils/RedisCpp.h"
#include "common/redis_utils/RedisPb.h"

class FollowApi {
  public:
    FollowApi(const std::string &user) : user_(user) {}; 

    int Follow(const std::string &target_user);
    int UnFollow(const std::string &target_user);
    int GetFollowTo(const std::string &target_user,
        std::vector<std::string> *users);
    int GetFollowFrom(const std::string &target_user,
        std::vector<std::string> *users);
    int IsFollowed(const std::string &target_user, bool *followed);
    int Block(const std::string &target_user);
    int UnBlock(const std::string &target_user);
    int IsBlocked(const std::string &from, const std::string &to,
        bool *blocked);

  private:
    RedisCode AddFollowAction(const FollowAction &follow_action);

  private:
    std::string user_;
};

#endif // COMMON_APP_FOLLOWAPI_H_
