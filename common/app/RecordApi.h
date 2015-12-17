#ifndef COMMON_APP_RECORDAPI_H_
#define COMMON_APP_RECORDAPI_H_

#include <string>
#include <vector>
#include "proto/record.pb.h"

class RecordApi {
  public:
    RecordApi() : page_count_(12) {}; 

    int Get(const std::string &id, Record *record);
    int Get(const std::string &id, StripRecord *record);
    int GetRecordOwner(const std::string &id, std::string *user);
    int Set(const Record &record);
    int CreateRecordId(std::string *id);
    int SetRecordPrivate(const std::string &id, bool is_private);
    int GetHomeRecord(const std::string &user,int page,
        std::vector<ExtRecord> *records);
    int GetActiveRecord(const std::string &user, int page,
        std::vector<ExtRecord> *records);
    int GetRecentRecord(const std::string &user, int page,
        std::vector<ExtRecord> *records);

    int LinkRecordToUserHome(const std::string &id, const std::string &user);
    int UnlinkRecordToUserHome(const std::string &id, const std::string &user);
    int LinkRecordToUserActive(const std::string &id, const std::string &user);
    int UnlinkRecordToUserActive(const std::string &id, const std::string &user);
    // int LinkRecordToUserRecent(const std::string &id, const std::string &user);

  private:
    int GetRecords(const std::string &key, int index_start,
        int index_stop, std::vector<std::string> *ids);
    int GetRecords(const std::vector<std::string> &keys,
        std::vector<ExtRecord> *records);

  private:
    int page_count_;
};

#endif // COMMON_APP_RECORDAPI_H_
