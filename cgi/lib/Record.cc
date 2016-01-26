#include "cgi/lib/Record.h"
#include <vector>
#include <iterator>
#include "common/app/CgiCode.h"
#include "common/app/ProfileApi.h"
#include "common/app/StatisticApi.h"
#include "common/app/InteractApi.h"
#include "common/tencent_img/TencentImg.h"
#include "thirdparty/plog/Log.h"
#include "service/record/client/RecordClient.h"

static void GetTokenValue(const std::string &text, const std::string &key,
    std::string *value) {
  std::size_t begin = text.find(key);
  if (begin == std::string::npos) {
    return;
  }
  std::size_t end = text.find(key, begin + key.length());
  if (begin == std::string::npos) {
    return;
  }

  if (end <= begin + key.length()) {
    return;
  }

  size_t token_length = end - begin - key.length(); 
  *value = text.substr(begin + key.length(), token_length);
  return value;
}

int64_t GetTimeFromString(const std::string& str_time) {
  struct tm* tmp_time = (struct tm*)malloc(sizeof(struct tm));  
  strptime(str_time.c_str(),"%Y%m%d%H%M%S",tmp_time);  
  time_t t = mktime(tmp_time);  
  free(tmp_time);  
  return t; 
}

static void GetSupperTimeAndAddress(const std::string text, int64_t *time,
    std::string *address) {
  std::string str_time;
  GetTokenValue(text, "__address__", address);
  GetTokenValue(text, "__time__", &str_time);
  if (str_time.empty()) {
    time = 0;
  } else {
    *time = GetTimeFromString(str_time);
    LOG_ERROR << "time stamp: " << *time;
  }

  return;
}

int Record::Create(const CreateRecordReq &req, CreateRecordRes *res) {
  std::string record_id;
  if (record_api_.CreateRecordId(&record_id) != kCgiCodeOk) {
    LOG_ERROR << "get record id failed! user: " << user_;
    return kCgiCodeSystemError;
  }
  int64_t super_time = 0;
  std::string super_address;
  GetSupperTimeAndAddress(req.text(), &super_time, &super_address);

  RoughRecord record;
  record.set_id(record_id);
  record.set_user(user_);
  record.set_time(super_time == 0 ? time(NULL) : super_time);
  record.set_weather(req.weather());
  record.set_mood(req.mood());
  record.set_status(req.status());
  record.set_text(req.text());
  record.set_address(super_address.empty() ? req.address() : super_address);
  record.set_is_delete(false);
  record.set_picture_count(req.picture());
  if (req.is_public() == "yes") {
    record.set_is_public(true);
  } else {
    record.set_is_public(false);
  }

  TencentImg tencent_img;
  res->set_appid(tencent_img.GetAppId());
  res->set_bucket(tencent_img.GetBucket());
  res->set_sign(tencent_img.GetPublicSign());
  std::vector<ImgConf> img_confs;
  GetImgConf(req.picture(), record_id, &img_confs);
  for (std::vector<ImgConf>::iterator iter = img_confs.begin();
      iter != img_confs.end(); ++iter) {
    ImgConf *pconf = res->add_img_conf();
    *pconf = *iter;
    std::string *download_url = record.add_pictures();
    *download_url = tencent_img.GetUrl(iter->fileid(), DownloadImgUrl);
  }

  if (record_api_.Set(record) != kCgiCodeOk) {
    LOG_ERROR << "Set record info failed! user: " << user_;
    return kCgiCodeSystemError;
  }

  RecordReq dispatch_req;
  dispatch_req.set_cmd(RECORD_CMD_ADD);
  dispatch_req.set_time(time(NULL));
  dispatch_req.set_id(record_id);
  RecordRes dispatch_res;
  RecordClient client;
  if (!(client.Send(dispatch_req, &dispatch_res))) {
    LOG_ERROR << "send record to dispatch server failed! user: " << user_
      << "record id: " << record_id << " error: " << client.Error();
  }

  // user incr record count statistic
  StatisticApi::AddRecord(user_);

  return kCgiCodeOk;
}


int Record::Delete(const DelRecordReq &req) {
  StatisticApi::DelRecord(user_);
  return record_api_.Del(req.id());
}

int Record::Get(const QueryRecordReq &req, ExtRecord *record) {
  RoughRecord rough_record;
  int ret = record_api_.Get(req.id(), &rough_record);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << "get record info failed! record id: " << req.id(); 
    return ret;
  }
  std::map<std::string, RoughRecord> records;
  records[req.id()] = rough_record;
  QueryRecordListRes res;
  if (BuildRecordListRes(records, 0, &res, true)
      == kCgiCodeSystemError) {
    return kCgiCodeSystemError;
  }

  if (res.records_size() > 0) {
    *record = res.records(0);
  }

  return kCgiCodeOk;
}

int Record::GetActive(const QueryRecordListReq &req, QueryRecordListRes *res) {
  std::map<std::string, RoughRecord> records;
  int ret = record_api_.GetActiveRecord(req.target_user(), req.page(), &records);
  if (ret == kCgiCodeSystemError) {
    LOG_ERROR << "get active record failed! user: " << user_;
    return kCgiCodeSystemError;
  }

  if (BuildRecordListRes(records, req.page(), res, true)
      == kCgiCodeSystemError) {
    return kCgiCodeSystemError;
  }

  return ret;
}

int Record::GetRecent(const QueryRecordListReq &req, QueryRecordListRes *res) {
  std::map<std::string, RoughRecord> records;
  int ret = record_api_.GetRecentRecord(req.target_user(), req.page(), &records);
  if (ret == kCgiCodeSystemError) {
    LOG_ERROR << "get recent record failed! user: " << user_;
    return kCgiCodeSystemError;
  }
  if (BuildRecordListRes(records, req.page(), res, true)
      == kCgiCodeSystemError) {
    return kCgiCodeSystemError;
  }

  return ret;
}

int Record::GetHome(const QueryRecordListReq &req, QueryRecordListRes *res) {
  std::map<std::string, RoughRecord> records;
  int ret = record_api_.GetHomeRecord(req.target_user(), req.page(), &records);
  if (ret == kCgiCodeSystemError) {
    LOG_ERROR << "get home record failed! user: " << user_;
    return kCgiCodeSystemError;
  }

  bool filter_private;
  if (req.target_user() != user_) {
    filter_private = true;
  } else {
    filter_private = false;
  }

  if (BuildRecordListRes(records, req.page(), res, filter_private)
      == kCgiCodeSystemError) {
    return kCgiCodeSystemError;
  }

  return ret;
}

int Record::AltPrivate(const AltRecordQrivateReq &req) {
  return record_api_.SetRecordPublic(req.id(),
      req.is_public() == "yes" ? true : false);
}

void Record::GetImgConf(int count, std::string record_id,
    std::vector<ImgConf> *confs) {
  TencentImg tencent_img;
  count = count > 9 ? 9 : count;
  std::vector<std::string> file_ids = tencent_img.GetFileId(user_, count);
  for (std::vector<std::string>::iterator iter = file_ids.begin();
      iter != file_ids.end(); iter++) {
    ImgConf conf;
    conf.set_fileid(*iter);
    std::string callback;
    callback += ("record=" + record_id);
    callback += (";fileid=" + *iter);
    conf.set_callback(callback);
    confs->push_back(conf);
  }
}

int Record::BuildRecordListRes(std::map<std::string, RoughRecord> &records,
    int page, QueryRecordListRes *res, bool filter_private) {
  std::map<std::string, RoughRecord>::iterator iter;
  std::set<std::string> users;
  std::vector<std::string> record_ids;

  for (iter = records.begin(); iter != records.end();) {
    if ((iter->second).is_delete()
        || (filter_private && !((iter->second).is_public()))) {
      iter = records.erase(iter);
    } else {
      (iter->second).set_web_url("http://182.254.220.116/h5/record.html?record_id=" + iter->first);
      users.insert((iter->second).user());
      record_ids.push_back(iter->first);
      ++iter;
    }
  }

  // Get user info
  ProfileApi profile_api;
  std::map<std::string, StripUserProfile> profiles;
  if (profile_api.MGet(users, &profiles) == kCgiCodeSystemError) {
    LOG_ERROR << "get home record failed! user: " << user_;
    return kCgiCodeSystemError;
  }

  // Statistic process
  // add view stat
  StatisticApi::ViewRecords(record_ids);

  // get interact stat
  std::map<std::string, RecordStat> stats;
  StatisticApi::GetRecordsStat(record_ids, &stats);

  // get like status
  InteractApi interact_api(user_);
  std::map<std::string, bool> is_likeds;
  interact_api.GetUserLikeRecordStatus(user_, record_ids, &is_likeds);


  std::map<std::string, RoughRecord>::reverse_iterator riter;
  for (riter = records.rbegin(); riter != records.rend(); ++riter) {
    ExtRecord ext_record;
    *(ext_record.mutable_record()) = riter->second;
    *(ext_record.mutable_user()) = profiles[(riter->second).user()];
    *(ext_record.mutable_interact()) = stats[riter->first];
    ext_record.set_is_liked(is_likeds[riter->first] ? "yes" : "no");
    *(res->add_records()) = ext_record;
  }

  res->set_page(page);
  return kCgiCodeOk;
}
