#ifndef COMMON_XG_PUSH_XGPUSH_H_
#define COMMON_XG_PUSH_XGPUSH_H_

#include <string>
#include <map>

// http://developer.qq.com/wiki/xg/%E6%9C%8D%E5%8A%A1%E7%AB%AFAPI%E6%8E%A5%E5%85%A5/Rest%20API%20%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97/Rest%20API%20%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97.html

class XGPush {
  public:
    XGPush();

    void SetParams(const std::map<std::string, std::string> params);
    void SetApiUrl(const std::string& url);
    void SetSecretKey(const std::string& secret_key);
    bool PushSingleAccount(const std::string &account, const std::string &msg);
    std::string ErrorMsg();

  private:
    std::string BuildSign();
    std::string BuildQueryString();

  private:
    std::map<std::string, std::string> params_;
    std::string secret_key_;
    std::string api_url_;
    std::string error_msg_;
};

#endif // COMMON_XG_PUSH_XGPUSH_H_
