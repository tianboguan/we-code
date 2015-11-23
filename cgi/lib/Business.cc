#include "cgi/lib/Business.h"
#include "cgi/lib/CacheKeys.h"
#include "thirdparty/plog/Log.h"

int Business::Disease(ElementList *res) {
  std::vector<std::string> values;
  if (redis_.Query("SMEMBERS", kDisease, &values) != RedisCodeOK) {
    LOG_ERROR << "SMEMBERS " << kDisease << " failed! err: " << redis_.Error();
    err_oss_ << "系统错误";
  }

  for (std::vector<std::string>::const_iterator iter = values.begin();
      iter != values.end(); ++iter) {
    std::string *element = res->add_elements(); 
    *element = *iter;
  }
  
  return 0;
}

int Business::Address(const AddressReq &req, AddressRes *res) {
  std::string key(kAddressPrefix + req.address_code());
  std::map<std::string, std::string> address;
  if (redis_.Query("HGETALL", key, &address) != RedisCodeOK) {
    LOG_ERROR << "HGETALL " << key << " failed! err: " << redis_.Error();
    err_oss_ << "系统错误";
  }

  ::google::protobuf::Map<::std::string, ::std::string> *res_map;
  res_map = res->mutable_address();
  std::map<std::string, std::string>::const_iterator iter;
  for (iter = address.begin(); iter != address.end(); ++iter) {
    (*res_map)[iter->first] = iter->second;
  }

  return 0;
}

int Business::Tag(const TagReq &req, TagRes *res) {
  std::map<std::string, std::vector<std::string> > res_data;
  int ret;
  if (req.tag_class() != "all") {
    ret = GetTags(req.tag_class(), &res_data[req.tag_class()]);
    if (ret != 0) {
      return -1;
    }
  } else {
    ret = GetTags(kTagWeather, &res_data[kTagWeather]);
    if (ret != 0) {
      return -1;
    }
    ret = GetTags(kTagMood, &res_data[kTagMood]);
    if (ret != 0) {
      return -1;
    }
    ret = GetTags(kTagStatus, &res_data[kTagStatus]);
    if (ret != 0) {
      return -1;
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
  
  return 0;
}

std::string Business::Error() {
  return 0;
}

int Business::GetTags(const std::string &key,
    std::vector<std::string> *values) {
  if (redis_.Query("SMEMBERS", key, values) != RedisCodeOK) {
    LOG_ERROR << "SMEMBERS" << key << " failed! err: " << redis_.Error();
    err_oss_ << "系统错误";
    return -1;
  }
  return 0;
}

