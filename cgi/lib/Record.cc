#include "cgi/lib/Record.h"
#include "cgi/lib/CgiCode.h"
#include "common/utils/string_utils.h"


int Record::Create(const CreateRecordReq &req, CreateRecordRes *res) {
  return kCgiCodeOk;
}

int Record::Delete(const DelRecordReq &req) {
  return kCgiCodeOk;
}

int Record::QueryDetail(const QueryRecordDetailReq &req, QueryRecordDetailRes *res) {
  return kCgiCodeOk;
}

int Record::QueryList(const QueryRecordListReq &req, QueryRecordListRes *res) {
  return kCgiCodeOk;
}

std::string Record::CreateRecordId() {
  return user_ + value_to_string(time(NULL));
}
