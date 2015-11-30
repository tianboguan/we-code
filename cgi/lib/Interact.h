#ifndef CGI_LIB_INTERACT_H_
#define CGI_LIB_INTERACT_H_

#include <string>
#include "common/redis_utils/RedisCpp.h"
#include "common/redis_utils/RedisPb.h"
#include "proto/CgiReq.pb.h"
#include "proto/CacheData.pb.h"

class Interact {
  public:
    Interact(std::string &user) : user_(user) {}; 

    int Like(const LikeReq &req);
    int Unlike(const UnlikeReq &req);
    int Commet(const CommentReq &req);
    int Uncomment(const UncommentReq &req);
    int InteractDetail(const InteractDetailReq &req, InteractDetailRes *res);
    int InteractNotice(const InteractNoticeReq &req, InteractNoticeRes *res);

  private:
    std::string user_;
    // std::ostringstream err_oss_;
    RedisCpp redis_;
    RedisStr2Pb<UserProfile> profile_redis_;
};

#endif // CGI_LIB_INTERACT_H_
