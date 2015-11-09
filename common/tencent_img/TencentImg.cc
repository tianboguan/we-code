#include "common/tencent_img/TencentImg.h"
#include "common/encode/HMAC_SHA1.h"
#include "common/encode/base64.h"
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;


string TencentImg::GetPublicSign() {
  ostringstream oss;
  oss << "a=" << app_id_
      << "&b=" << bucket_
      << "&k=" << secret_id_
      << "&e=" << time(NULL) + 864000
      << "&t=" << time(NULL)
      << "&r=" << rand() % 100000
      << "&u=0&f=";
  return CreateSign(oss.str());
}

string TencentImg::GetPrivateSign(const string &file_id) {
  ostringstream oss;
  oss << "a=" << app_id_
      << "&b=" << bucket_
      << "&k=" << secret_id_
      << "&e=0&t=" << time(NULL)
      << "&r=" << rand() % 100000
      << "&u=0&f=" << file_id;
  return CreateSign(oss.str());
}

vector<string> TencentImg::GetFileId(const string &user_name, int count) {
  vector<string> urls;
  time_t now = time(NULL);
  for (int i = 0;  i < count; i++) {
    ostringstream oss;
    oss << user_name << "_" << now << "_" << i;
    urls.push_back(oss.str());
  }
  return urls;
}

string TencentImg::GetUrl(const string &file_id, ImgUrlType type) {
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

string TencentImg::CreateSign(string orignal) {
  CHMAC_SHA1 chmac_sha1; 
  BYTE digest[20];
  chmac_sha1.HMAC_SHA1(orignal.c_str(), orignal.length(),
                       secret_key_.c_str(), secret_key_.length(),
                       digest);
  string base64_orignal = string(digest) + "." + orignal;
  return base64_encode(base64_orignal.c_str(), base64_orignal.size());
}

