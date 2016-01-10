#ifndef COMMON_APP_RECORDAPI_H_
#define COMMON_APP_RECORDAPI_H_

#include <string>
#include <vector>
#include "proto/record.pb.h"

class RecordApi {
  public:
    RecordApi(int page_count = 12) : page_count_(page_count) {}; 

    int Get(const std::string &id, RoughRecord *record);
    int Get(const std::string &id, StripRecord *record);
    int Del(const std::string &id);
    int HardDel(const std::string &id);
    int Set(const RoughRecord &record);
    int SetRecordPublic(const std::string &id, bool is_public);
    int GetRecordOwner(const std::string &id, std::string *user);
    int CreateRecordId(std::string *id);
    int GetHomeRecord(const std::string &user,int page,
        std::map<std::string, RoughRecord> *records);
    int GetActiveRecord(const std::string &user, int page,
        std::map<std::string, RoughRecord> *records);
    int GetRecentRecord(const std::string &user, int page,
        std::map<std::string, RoughRecord> *records);

    int LinkRecordToUserHome(const std::string &id, const std::string &user);
    int UnlinkRecordToUserHome(const std::string &id, const std::string &user);
    int LinkRecordToUserActive(const std::string &id, const std::string &user);
    int UnlinkRecordToUserActive(const std::string &id, const std::string &user);
    int LinkRecordToRecent(const std::string &id);

  private:
    int GetRecords(const std::string &key, int index_start,
        int index_stop, std::vector<std::string> *ids);
    int GetRecords(const std::vector<std::string> &ids,
        std::map<std::string, RoughRecord> *records);

  private:
    int page_count_;
};

#endif // COMMON_APP_RECORDAPI_H_
