#include <iterator>
#include "common/cgi_utils/CgiHandle.h"
#include "thirdparty/plog/Log.h"

using namespace cgicc;

CgiHandle::CgiHandle() {
  method_ = form_data_.getEnvironment().getRequestMethod();

  const_form_iterator iter;
  for(iter = form_data_.getElements().begin(); 
      iter != form_data_.getElements().end(); 
      ++iter) {
    query_kv_[iter->getName()] = iter->getValue();
  }
}

std::string CgiHandle::operator [] (const std::string& key) {
  std::map<std::string, std::string>::const_iterator iter;
  iter = query_kv_.find(key);
  if (iter != query_kv_.end()) {
    return iter->second;
  } else {
    return "";
  }
}

const std::map<std::string, std::string>& CgiHandle::GetParams() {
  std::string result;
  std::map<std::string, std::string>::iterator iter;
  for (iter = query_kv_.begin(); iter != query_kv_.end(); ++iter) {
    result += iter->first + " -> " + iter->second + ", ";
  }

  
  LOG_ERROR << "got a request:";
  LOG_ERROR << "---------------------------";

  LOG_ERROR << result;

  return query_kv_;
}

std::string CgiHandle::GetMethod() {
  return method_;
}
