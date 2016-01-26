#ifndef COMMON_APP_INTERACTAPI_H_
#define COMMON_APP_INTERACTAPI_H_

#include <string>
#include <vector>
#include "proto/interact.pb.h"

class InteractApi {
  public:
    InteractApi(const std::string &user) : user_(user), page_count_(28) {}; 

    int Like(const std::string &record_id);
    int Unlike(const std::string &record_id);
    int GetUserLikeRecordStatus(const std::string &user,
        const std::string &record_id, bool *is_liked);
    int GetUserLikeRecordStatus(const std::string &user,
        const std::vector<std::string> &records,
        std::map<std::string, bool> *is_likeds);
    int Comment(const std::string &record_id, 
                const std::string &text,
                const std::string &target_interact_id = "");
    int Uncomment(const std::string &comment_id, const std::string &record_id);
    int GetRecordInteracts(const std::string &record_id,
        std::vector<ExtInteract> *interacts);
    int GetUserNoticeInteracts(int32_t page,
        std::vector<ExtInteract> *interacts);
    int GetUserReceivedInteracts(int32_t page,
        std::vector<ExtInteract> *interacts);
    int GetUserSendedInteracts(int32_t page,
        std::vector<ExtInteract> *interacts);
    int ClearRecordInteracts(const std::string &id);
    int ClearUserNoticeInteracts();
    int ClearUserReceivedInteracts();
    int ClearUserSendedInteracts();

  private:
    int AddUserLikeStatus(const std::string &record_id,
        const std::string &user);
    int DeleteUserLikeStatus(const std::string &record_id,
        const std::string &user);
    int CreateInterId(std::string *id);
    int DispatchInteract(const RoughInteract &interact);
    int DelInteract(const std::string &interact_id,
        const std::string &record_id);

    int GetInteracts(const std::string &key, int32_t index_start,
        int32_t index_stop, std::vector<std::string> *ids);
    int GetInteracts(const std::vector<std::string> &keys,
        std::vector<ExtInteract> *interacts);
    int Convert(const RoughInteract &from, ExtInteract *to);
    int ExtInteractUser(const std::string &user, ExtInteract *interact);
    int ExtInteractRecord(const std::string &user, ExtInteract *interact);

    int SetRoughInteract(const RoughInteract &interact);
    int GetRoughInteract(const std::string &interact_id,
        RoughInteract *interact);

    int SetExtUserInteract(const ExtInteract &interact);
    int GetExtUserInteract(const std::string &id, ExtInteract *interact);

    int SetExtRecordInteract(const ExtInteract &interact);
    int GetExtRecordInteract(const std::string &id, ExtInteract *interact);

    int LinkInteractToRecord(const std::string &interact, const std::string &record);
    int UnlinkInteractFromRecord(const std::string &interact, const std::string &record);

    int LinkInteractToUserInteractTo(const std::string &interact, const std::string &user);
    int UnlinkInteractFromUserInteractTo(const std::string &interact, const std::string &user);
    int LinkInteractToUserInteractFrom(const std::string &interact, const std::string &user);
    int UnlinkInteractFromUserInteractFrom(const std::string &interact, const std::string &user);
    int LinkInteractToUserInteractNotice(const std::string &interact, const std::string &user);

  private:
    std::string user_;
    int32_t page_count_;
};

#endif // COMMON_APP_INTERACTAPI_H_
