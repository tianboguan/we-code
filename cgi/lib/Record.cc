#include "cgi/lib/Record.h"
#include <vector>
#include <iterator>
#include "common/app/CgiCode.h"
#include "common/tencent_img/TencentImg.h"
#include "thirdparty/plog/Log.h"

int Record::Create(const CreateRecordReq &req, CreateRecordRes *res) {
  std::string record_id;
  if (record_api_.CreateRecordId(&record_id) != kCgiCodeOk) {
    LOG_ERROR << "get record id failed! user: " << user_;
    return kCgiCodeSystemError;
  };

  RoughRecord record;
  record.set_id(record_id);
  record.set_user(user_);
  record.set_time(time(NULL));
  record.set_weather(req.weather());
  record.set_mood(req.mood());
  record.set_status(req.status());
  record.set_text(req.text());
  record.set_address(req.address());
  record.set_is_delete(false);
  record.set_picture_count(req.picture());
  if (req.is_public() == "yes") {
    record.set_is_public(true);
  } else {
    record.set_is_public(false);
  }

  if (record_api_.Set(record) != kCgiCodeOk) {
    LOG_ERROR << "Set record info failed! user: " << user_;
    return kCgiCodeSystemError;
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
  }

  // TODO send to server dispatch
  
  return kCgiCodeOk;
}


int Record::Delete(const DelRecordReq &req) {
  return record_api_.Del(req.id());
}

int Record::Get(const QueryRecordReq &req, ExtRecord *record) {
  // TODO
  return kCgiCodeOk;
}

int Record::GetActive(const QueryRecordListReq &req, QueryRecordListRes *res) {
  // TODO
  return kCgiCodeOk;
}

int Record::GetRecent(const QueryRecordListReq &req, QueryRecordListRes *res) {
  // TODO
  return kCgiCodeOk;
}

int Record::GetHome(const QueryRecordListReq &req, QueryRecordListRes *res) {
  // TODO
  return kCgiCodeOk;
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
    callback += ("fileid=" + *iter);
    conf.set_callback(callback);
    confs->push_back(conf);
  }
}
