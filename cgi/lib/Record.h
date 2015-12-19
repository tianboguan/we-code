#ifndef CGI_LIB_RECORD_H_
#define CGI_LIB_RECORD_H_

#include <string>
#include "proto/CgiReq.pb.h"
#include "common/app/RecordApi.h"
#include <map>

class Record {
  public:
    Record (const std::string &user) : user_(user) {}; 

    int Create(const CreateRecordReq &req, CreateRecordRes *res);
    int Delete(const DelRecordReq &req);
    int Get(const QueryRecordReq &req, ExtRecord *record);
    int GetActive(const QueryRecordListReq &req, QueryRecordListRes *res);
    int GetRecent(const QueryRecordListReq &req, QueryRecordListRes *res);
    int GetHome(const QueryRecordListReq &req, QueryRecordListRes *res);
    int AltPrivate(const AltRecordQrivateReq &req);

  private:
    int BuildRecordListRes(std::map<std::string, RoughRecord> &records,
        QueryRecordListRes *res, bool filter_private = false);
    void GetImgConf(int count, std::string record_id, std::vector<ImgConf> *confs);

  private:
    std::string user_;
    RecordApi record_api_;
};

#endif // CGI_LIB_RECORD_H_
