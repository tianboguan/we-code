#include "common/xg_push/XGPush.h"
#include "common/utils/StringUtils.h"
#include "common/encode/md5.h"
#include <curl/curl.h>

XGPush::XGPush() {
  params_["access_id"] = "2100174978";
  params_["timestamp"] = value_to_string(time(NULL));
  params_["valid_time"] = "600";
  secret_key_ = "cfbd27e73b28ff0b668e7dedfa58e67f";
  api_url_ = "openapi.xg.qq.com/v2/push/single_account";
}

void XGPush::SetParams(const std::map<std::string, std::string> params) {
  for (std::map<std::string, std::string>::const_iterator iter = params.begin();
      iter != params.end(); ++iter) {
    params_[iter->first] = iter->second;
  }
}
void XGPush::SetApiUrl(const std::string& url) {
  api_url_ = url;
}
void XGPush::SetSecretKey(const std::string& secret_key) {
  secret_key_ = secret_key;
}

bool XGPush::PushSingleAccount(const std::string &account, const std::string &msg) {
  params_["account"] = account;
  params_["message_type"] = "2";
  params_["message"] = msg;
  params_["expire_time"] = "86400" ;
  params_["multi_pkg"] = "1" ;
  std::string sign = BuildSign();
  params_["sign"] = sign;
  std::string query_string = BuildQueryString();

  bool ret = false;
  // curl example:
  // http://curl.haxx.se/libcurl/c/http-post.html
  CURL *curl;
  CURLcode res;

  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
     * just as well be a https:// URL if that is what should receive the
     * data. */ 
    curl_easy_setopt(curl, CURLOPT_URL, ("http://" + api_url_).c_str());
    /* Now specify the POST data */ 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query_string.c_str());

    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK) {
      error_msg_ = "curl_easy_perform() failed: " + string(curl_easy_strerror(res));
      ret = false;
    } else {
      ret = true;
    }
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return ret;
}

std::string XGPush::ErrorMsg() {
  return error_msg_;
}

std::string XGPush::BuildSign() {
  std::string build_string;
  build_string += "POST";
  build_string += api_url_;
  for (std::map<std::string, std::string>::const_iterator iter = params_.begin();
      iter != params_.end(); ++iter) {
    build_string += iter->first + "=" + iter->second;
  }
  build_string += secret_key_;

  MD5 md5(build_string);
  return md5.toStr();
}

std::string XGPush::BuildQueryString() {
  std::vector<std::string> tokens;
  for (std::map<std::string, std::string>::const_iterator iter = params_.begin();
      iter != params_.end(); ++iter) {
    tokens.push_back(iter->first + "=" + iter->second);
  }
  return vector_to_string(tokens, "&");
}


