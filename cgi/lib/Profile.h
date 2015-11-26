#ifndef CGI_LIB_PROFILE_H_
#define CGI_LIB_PROFILE_H_

#include <string>
#include <map>
#include "common/redis_utils/RedisPb.h"
#include "proto/CgiReq.pb.h"
#include "proto/CacheData.pb.h"

class Profile {
  public:
    Profile(const std::string &user) : user_(user) {}; 

    int Add(const UserProfile &profile);
    int Alt(const UserProfile &profile);
    int AltHead(ImgRes *res);
    int Query(const std::string &target_user, UserProfile *profile);
    int Query(const std::string &target_user, UserStat *stat);

  private:
    // int GetUserProfile(const std::vector<string> &users, FollowListRes *res);

  private:
    std::string user_;
    RedisCpp redis_;
    RedisStr2Pb<UserProfile> profile_redis_;
    RedisStr2Pb<UserStat> stat_redis_;
};

#endif // CGI_LIB_PROFILE_H_
