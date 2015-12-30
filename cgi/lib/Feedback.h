#ifndef CGI_LIB_FEEDBACK_H_
#define CGI_LIB_FEEDBACK_H_

#include <string>
#include "proto/CgiReq.pb.h"
// #include "common/app/RecordApi.h"
// #include <map>

class Feedback {
  public:
    Feedback(const std::string &user) : user_(user) {}; 
    int Supply(const FeedbackReq &req, CreateRecordRes *res);

  private:
    void GetImgConf(int count, std::string feedback_id,
        std::vector<ImgConf> *confs);
  private:
    std::string user_;
};

#endif // CGI_LIB_FEEDBACK_H_
