#ifndef COMMON_APP_STATISTICAPI_H_
#define COMMON_APP_STATISTICAPI_H_

#include <string>
#include <vector>
#include "proto/statistic.pb.h"

class StatisticApi {
  public:
    StatisticApi() {}; 

    // record stat
    static int ViewRecord(const std::string &id);
    static int ViewRecords(const std::vector<std::string> &ids);
    static int LikeRecord(const std::string &id);
    static int UnlikeRecord(const std::string &id);
    static int CommentRecord(const std::string &id); 
    static int UncommentRecord(const std::string &id); 
    static int GetRecordStat(const std::string &id, RecordStat *stat);
    static int GetRecordsStat(const std::vector<std::string> &ids,
       std::map<std::string, RecordStat> *stats);

    // user stat
    static int AddRecord(const std::string &user);
    static int DelRecord(const std::string &user);
    static int UserLike(const std::string &user);
    static int UserLiked(const std::string &user);
    static int UserComment(const std::string &user);
    static int UserCommented(const std::string &user);
    static int UpdateUserFollow(const std::string &user);
    static int UpdateUserFollowed(const std::string &user);
    // static int UserUnfollow(const std::string &user);
    // static int UserUnfollowed(const std::string &user);
    static int GetUserStat(const std::string &user, UserStat *stat);
};

#endif // COMMON_APP_STATISTICAPI_H_
