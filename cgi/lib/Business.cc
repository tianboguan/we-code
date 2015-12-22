#include <map>
#include "cgi/lib/Business.h"
#include "common/app/RedisKeys.h"
#include "thirdparty/plog/Log.h"
#include "common/app/CgiCode.h"
#include "proto/business.pb.h"
#include "common/utils/PbUtils.h"

int Business::Disease(DiseaseRes *res) {
#if 1
  std::string conf_file("./disease.conf");
  int ret = ParsePbConfigFromFile(conf_file, res);
  if (ret != 0) {
    LOG_ERROR << "parse addr config file error!";
    return kCgiCodeSystemError;
  }
#else
  std::vector<std::string> values;
  if (redis_.Query("SMEMBERS", GetDiseaseKey(), &values) != RedisCodeOK) {
    return kCgiCodeSystemError;
  }

  for (std::vector<std::string>::const_iterator iter = values.begin();
      iter != values.end(); ++iter) {
    std::string *element = res->add_elements(); 
    *element = *iter;
  }
#endif
  
  return kCgiCodeOk;
}

int Business::Address(const AddressReq &req, AddressRes *res) {
  AddressConf addr_conf;
  std::string conf_file("./address.conf");
  int ret = ParsePbConfigFromFile(conf_file, &addr_conf);
  if (ret != 0) {
    LOG_ERROR << "parse addr config file error!";
    return kCgiCodeSystemError;
  }

  ::google::protobuf::Map<::std::string, AddressNode>::const_iterator iter;
  iter = addr_conf.code2addr().find(req.address_code());
  if (iter == addr_conf.code2addr().end()) {
    LOG_ERROR << "invalid address code!" << req.address_code();
    return kCgiCodeInvalidAddressCode;
  }

  ::google::protobuf::Map<::std::string, ::std::string> *res_map;
  res_map = res->mutable_address();
  *res_map = (iter->second).childs();

  return kCgiCodeOk;
}

int Business::Tag(const TagReq &req, TagRes *res) {
#if 1
  std::string conf_file("./tag.conf");
  int ret = ParsePbConfigFromFile(conf_file, res);
  if (ret != 0) {
    LOG_ERROR << "parse addr config file error!";
    return kCgiCodeSystemError;
  }
#else

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

  // AddressConf addr_conf;
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
#endif
  
  return kCgiCodeOk;
}

int Business::GetTags(const std::string &tag_name,
    std::vector<std::string> *values) {
  return redis_.Query("SMEMBERS", GetTagKey(tag_name), values) == RedisCodeOK ?
    kCgiCodeOk : kCgiCodeSystemError;
}

