#ifndef CGI_LIB_BUSINESS_H_
#define CGI_LIB_BUSINESS_H_

#include <string>
#include <sstream>
#include "common/redis_utils/RedisCpp.h"
#include "proto/CgiReq.pb.h"

const std::string kTagWeather = "weather";
const std::string kTagMood = "mood";
const std::string kTagStatus = "status";

class Business {
  public:
    Business() {}; 

    // int Disease(ElementList *res);
    int Disease(DiseaseRes *res);
    int Address(const AddressReq &req,AddressRes *res);
    // int Address(const AddressReq &req,
    //    ::google::protobuf::Map<::std::string, ::std::string> *res_map);
    int Tag(const TagReq &req, TagRes *res);

  private:
    int GetTags(const std::string &key, std::vector<std::string> *values);

  private:
    // int i_;
    RedisCpp redis_;
};

#endif // CGI_LIB_BUSINESS_H_
