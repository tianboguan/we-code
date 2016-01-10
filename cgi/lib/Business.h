#ifndef CGI_LIB_BUSINESS_H_
#define CGI_LIB_BUSINESS_H_

#include <string>
#include <sstream>
#include "proto/CgiReq.pb.h"

const std::string kTagWeather = "weather";
const std::string kTagMood = "mood";
const std::string kTagStatus = "status";

class Business {
  public:
    Business() {}; 

    int Disease(DiseaseRes *res);
    int Address(const AddressReq &req,AddressRes *res);
    int Tag(const TagReq &req, TagRes *res);
};

#endif // CGI_LIB_BUSINESS_H_
