#include "cgi/lib/Business.h"
#include "common/app/RedisKeys.h"
#include "thirdparty/plog/Log.h"
#include "common/app/CgiCode.h"

int Business::Disease(ElementList *res) {
  std::vector<std::string> values;
  if (redis_.Query("SMEMBERS", GetDiseaseKey(), &values) != RedisCodeOK) {
    return kCgiCodeSystemError;
  }

  for (std::vector<std::string>::const_iterator iter = values.begin();
      iter != values.end(); ++iter) {
    std::string *element = res->add_elements(); 
    *element = *iter;
  }
  
  return kCgiCodeOk;
}

int Business::Address(const AddressReq &req, AddressRes *res) {
  std::map<std::string, std::string> address;
  if (redis_.Query("HGETALL", GetAddressKey(req.address_code()), &address)
      != RedisCodeOK) {
    return kCgiCodeSystemError;
  }

  ::google::protobuf::Map<::std::string, ::std::string> *res_map;
  res_map = res->mutable_address();
  std::map<std::string, std::string>::const_iterator iter;
  for (iter = address.begin(); iter != address.end(); ++iter) {
    (*res_map)[iter->first] = iter->second;
  }

  return kCgiCodeOk;
}

int Business::Tag(const TagReq &req, TagRes *res) {
  std::map<std::string, std::vector<std::string> > res_data;
  int ret;
  if (req.tag_class() != "all") {
    ret = GetTags(req.tag_class(), &res_data[req.tag_class()]);
    if (ret != kCgiCodeOk) {
      return ret;
    }
  } else {
    ret = GetTags(kTagWeather, &res_data[kTagWeather]);
    if (ret != kCgiCodeOk) {
      return ret;
    }
    ret = GetTags(kTagMood, &res_data[kTagMood]);
    if (ret != kCgiCodeOk) {
      return ret;
    }
    ret = GetTags(kTagStatus, &res_data[kTagStatus]);
    if (ret != kCgiCodeOk) {
      return ret;
    }
  }

  ::google::protobuf::Map< ::std::string, ::ElementList> *res_tags;
  res_tags = res->mutable_tags();
  std::map<std::string, std::vector<std::string> >::const_iterator iter1;
  std::vector<std::string>::const_iterator iter2;
  for (iter1 = res_data.begin(); iter1 != res_data.end();  ++iter1) {
    ElementList elements;
    for (iter2 = iter1->second.begin(); iter2 != iter1->second.end(); ++iter2) {
      std::string *element = elements.add_elements();
      *element = *iter2;
    }
    (*res_tags)[iter1->first] = elements;
  }
  
  return kCgiCodeOk;
}

int Business::GetTags(const std::string &tag_name,
    std::vector<std::string> *values) {
  return redis_.Query("SMEMBERS", GetTagKey(tag_name), values) == RedisCodeOK ?
    kCgiCodeOk : kCgiCodeSystemError;
}

