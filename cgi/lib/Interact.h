#ifndef CGI_LIB_INTERACT_H_
#define CGI_LIB_INTERACT_H_

#include <string>
#include "proto/CgiReq.pb.h"
#include "common/app/InteractApi.h"
// #include "proto/interact.pb.h"
//#include "proto/CacheData.pb.h"

class Interact {
  public:
    Interact(const std::string &user) : api_(user) {}; 

    int Like(const LikeReq &req);
    int Unlike(const UnlikeReq &req);
    int Comment(const CommentReq &req);
    int Uncomment(const UncommentReq &req);
    int GetRecordInteracts(const RecordInteractReq &req,
        RecordInteractRes *res);
    int GetNoticeInteracts(const UserInteractReq &req,
        UserInteractRes *res);
    int GetSendedInteracts(const UserInteractReq &req,
        UserInteractRes *res);
    int GetReceivedInteracts(const UserInteractReq &req,
        UserInteractRes *res);

  private:
    void BuildUserInteracts(const std::vector<ExtInteract> &interacts,
        int32_t page, UserInteractRes *res);

  private:
    InteractApi api_;
};

#endif // CGI_LIB_INTERACT_H_
