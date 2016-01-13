#ifndef CGI_LIB_PROFILE_H_
#define CGI_LIB_PROFILE_H_

#include <string>
#include <map>
#include "proto/CgiReq.pb.h"
#include "proto/profile.pb.h"


class Profile {
  public:
    Profile(const std::string &user) : user_(user) {}; 

    int Alt(const AltProfileReq &req);
    int AltHead(ImgRes *res);
    int Query(const QueryProfileReq &req, UserProfile *profile);
    int Query(const std::string &target_user, UserStat *stat);

  private:
    // int GetUserProfile(const std::vector<string> &users, FollowListRes *res);

  private:
    std::string user_;
};

#endif // CGI_LIB_PROFILE_H_
