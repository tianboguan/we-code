#include "cgi/lib/Feedback.h"
#include <vector>
#include <iterator>
#include "common/app/CgiCode.h"
//#include "common/app/ProfileApi.h"
#include "common/tencent_img/TencentImg.h"
#include "thirdparty/plog/Log.h"
//#include "service/record/client/RecordClient.h"

int Feedback::Supply(const FeedbackReq &req, CreateRecordRes *res) {
  TencentImg tencent_img;
  res->set_appid(tencent_img.GetAppId());
  res->set_bucket(tencent_img.GetBucket());
  res->set_sign(tencent_img.GetPublicSign());
  std::vector<ImgConf> img_confs;
  // TODO feedback id
  std::string feedback_id = "";
  GetImgConf(req.img_count(), feedback_id, &img_confs);
  for (std::vector<ImgConf>::iterator iter = img_confs.begin();
      iter != img_confs.end(); ++iter) {
    ImgConf *pconf = res->add_img_conf();
    *pconf = *iter;
  }

  // TODO store feedback
#if 0
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
#endif

  return kCgiCodeOk;
}



void Feedback::GetImgConf(int count, std::string feedback_id,
    std::vector<ImgConf> *confs) {
  TencentImg tencent_img;
  count = count > 9 ? 9 : count;
  std::vector<std::string> file_ids = tencent_img.GetFileId(user_, count);
  for (std::vector<std::string>::iterator iter = file_ids.begin();
      iter != file_ids.end(); iter++) {
    ImgConf conf;
    conf.set_fileid(*iter);
    std::string callback;
    callback += ("feedback=" + feedback_id);
    callback += (";fileid=" + *iter);
    conf.set_callback(callback);
    confs->push_back(conf);
  }
}

