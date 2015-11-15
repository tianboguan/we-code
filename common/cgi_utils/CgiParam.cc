#include <string>
#include <iterator>
#include <map>
#include "common/cgi_utils/CgiParam.h"

using namespace std;
using namespace cgicc;

CgiParam::CgiParam() {
  method_ = form_data_.getEnvironment().getRequestMethod();

  const_form_iterator iter;
  for(iter = form_data_.getElements().begin(); 
      iter != form_data_.getElements().end(); 
      ++iter) {
    query_kv_[iter->getName()] = iter->getValue();
  }
}

string CgiParam::operator [] (const string& key) {
  map<string, string>::const_iterator iter;
  iter = query_kv_.find(key);
  if (iter != query_kv_.end()) {
    return iter->second;
  } else {
    return "";
  }
}

const map<string, string>& CgiParam::GetParams() {
  return query_kv_;
}

string CgiParam::GetMethod() {
  return method_;
}
