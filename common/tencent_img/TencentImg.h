#ifndef __TENCENTIMG_H__
#define __TENCENTIMG_H__

#include <string>
#include <vector>

// using namespace std;

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
    TencentImg(); 

    std::string GetAppId();
    std::string GetBucket();
    std::string GetPublicSign();
    std::string GetPrivateSign(const std::string &file_id);
    std::vector<std::string> GetFileId(const std::string &user_name, int count);
    std::string GetUrl(const std::string &file_id, ImgUrlType type);

  private:
    std::string CreateSign(std::string orignal);

  private:
    std::string app_id_;
    std::string bucket_;
    std::string secret_key_;
    std::string secret_id_;
    std::string base_url_;
    std::string download_base_url_;
};

#endif  /*__TENCENTIMG_H__*/
