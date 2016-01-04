#include "cgi/lib/Feedback.h"
#include <vector>
#include <iterator>
#include "common/app/CgiCode.h"
#include "common/tencent_img/TencentImg.h"
#include "thirdparty/plog/Log.h"

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

