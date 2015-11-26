#include "common/tencent_img/TencentImg.h"
#include "common/encode/HMAC_SHA1.h"
#include "common/encode/base64.h"
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <iostream>

TencentImg::TencentImg() {
  app_id_ = "10010517"; 
  bucket_ = "feelings";
  secret_id_ = "AKIDCGWoeO7dDu2hiIeEVy6p7qlj3osHJH4P";
  secret_key_ = "BI8L0haIRG3TL8TthAGgtNQ2RBhVrIa4";
  base_url_ = "http://web.image.myqcloud.com/photos/v2/10010517/feelings/0/";
  download_base_url_ = "http://feelings-10010517.image.myqcloud.com/";
}

std::string TencentImg::GetAppId() {
  return app_id_;
}

std::string TencentImg::GetBucket() {
  return bucket_;
}

std::string TencentImg::GetPublicSign() {
  std::ostringstream oss;
  oss << "a=" << app_id_
    << "&b=" << bucket_
    << "&k=" << secret_id_
    << "&e=" << time(NULL) + 864000
    << "&t=" << time(NULL)
    << "&r=" << rand() % 100000
    << "&u=0&f=";
  return CreateSign(oss.str());
}

std::string TencentImg::GetPrivateSign(const std::string &file_id) {
  std::ostringstream oss;
  oss << "a=" << app_id_
    << "&b=" << bucket_
    << "&k=" << secret_id_
    << "&e=0&t=" << time(NULL)
    << "&r=" << rand() % 100000
    << "&u=0&f=" << file_id;
  return CreateSign(oss.str());
}

std::vector<std::string> TencentImg::GetFileId(const std::string &user_name, int count) {
  std::vector<std::string> urls;
  time_t now = time(NULL);
  for (int i = 0;  i < count; i++) {
    std::ostringstream oss;
    oss << user_name << "_" << now << "_" << i << ".jpg";
    urls.push_back(oss.str());
  }
  return urls;
}

std::string TencentImg::GetUrl(const std::string &file_id, ImgUrlType type) {
  switch (type) {
    case UploadImgUrl:
      return base_url_ + file_id;
    case CopyImgUrl:
      return base_url_ + file_id + "/copy";
    case QueryImgUrl:
      return base_url_ + file_id;
    case DeleteImgUrl:
      return base_url_ + file_id + "/del";
    case DownloadImgUrl:
      return download_base_url_ + file_id;
  }
  return "";
}

std::string TencentImg::CreateSign(std::string orignal) {
  CHMAC_SHA1 chmac_sha1; 
  BYTE digest[20];
  chmac_sha1.HMAC_SHA1(orignal.c_str(), orignal.length(),
      secret_key_.c_str(), secret_key_.length(),
      digest);
  char  base64_orignal[1024] = {0};
  memcpy(base64_orignal, digest, 20);
  memcpy(base64_orignal + 20, orignal.c_str(), orignal.length());
  return base64_encode(base64_orignal, orignal.length() + 20);
}

