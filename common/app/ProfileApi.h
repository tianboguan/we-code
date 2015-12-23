#ifndef COMMON_APP_PROFILEAPI_H_
#define COMMON_APP_PROFILEAPI_H_

#include <string>
#include <map>
#include "proto/CgiReq.pb.h"

class ProfileApi {
  public:
    int Create(const std::string &user, bool check = true);
    int Create(const UserProfile &profile, bool check = true);
    int Update(const std::string &user, const UserProfile &profile);
    int Get(const std::string &user, UserProfile *profile);
    int Get(const std::string &user, StripUserProfile *strip);
    int MGet(const std::set<std::string> &users,
        std::map<std::string, UserProfile> *profiles);
    int MGet(const std::set<std::string> &users,
        std::map<std::string, StripUserProfile> *strips);
    int Del(const std::string &user);

    void Profile2Base(const UserProfile &profile, StripUserProfile *strip);
};

#endif // COMMON_APP_PROFILEAPI_H_
