#ifndef CGI_LIB_BUSINESS_H_
#define CGI_LIB_BUSINESS_H_

#include <string>
#include <sstream>
#include "common/redis_utils/RedisCpp.h"
#include "proto/CgiReq.pb.h"

class Business {
  public:
    Business(); 

    int Disease(ElementList *res);
    int Address(const AddressReq &req,AddressRes *res);
    int Tag(const TagReq &req, TagRes *res);

    std::string Error();

  private:
    int GetTags(const std::string &key, std::vector<std::string> *values);

  private:
    std::string user_;
    std::ostringstream err_oss_;
    RedisCpp redis_;
};

#endif // CGI_LIB_BUSINESS_H_
