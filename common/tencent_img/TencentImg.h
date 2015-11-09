#ifndef __TENCENTIMG_H__
#define __TENCENTIMG_H__

#include <string>
#include <vector>

using namespace std;

// http://www.qcloud.com/wiki/%E4%B8%87%E8%B1%A1%E4%BC%98%E5%9B%BERestful_API%E6%96%87%E6%A1%A3#7.09.E5.9B.BE.E7.89.87.E4.B8.8B.E8.BD.BD
enum ImgUrlType {
  UploadImgUrl = 0,
  CopyImgUrl = 1,
  QueryImgUrl = 2,
  DeleteImgUrl = 3,
  DownloadImgUrl = 4
};

class TencentImg {
  public:
    TencentImg() {
      app_id_ = "10010517"; 
      bucket_ = "feelings";
      secret_id_ = "AKIDCGWoeO7dDu2hiIeEVy6p7qlj3osHJH4P";
      secret_key_ = "BI8L0haIRG3TL8TthAGgtNQ2RBhVrIa4";
      base_url_ = "http://web.image.myqcloud.com/photos/v2/10010517/feelings/0/";
      download_base_url_ = "http://feelings-10010517.image.myqcloud.com/";
    }

    string GetPublicSign();
    string GetPrivateSign(const string &file_id);
    vector<string> GetFileId(const string &user_name, int count);
    string GetUrl(const string &file_id, ImgUrlType type);

  private:
    string CreateSign(string orignal);

  private:
    string app_id_;
    string bucket_;
    string secret_key_;
    string secret_id_;
    string base_url_;
    string download_base_url_;
};

#endif  /*__TENCENTIMG_H__*/
