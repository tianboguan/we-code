#include "cgi/lib/Business.h"
#include "thirdparty/plog/Log.h"
#include "common/app/CgiCode.h"
#include "proto/business.pb.h"
#include "common/utils/PbUtils.h"

int Business::Disease(DiseaseRes *res) {
  std::string conf_file("./disease.conf");
  int ret = ParsePbConfigFromFile(conf_file, res);
  if (ret != 0) {
    LOG_ERROR << "parse addr config file error!";
    return kCgiCodeSystemError;
  }
  
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
  std::string conf_file("./tag.conf");
  int ret = ParsePbConfigFromFile(conf_file, res);
  if (ret != 0) {
    LOG_ERROR << "parse addr config file error!";
    return kCgiCodeSystemError;
  }
  
  return kCgiCodeOk;
}

