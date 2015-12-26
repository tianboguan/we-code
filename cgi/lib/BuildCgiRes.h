#ifndef CGI_LIB_BUILDCGIRES_H_
#define CGI_LIB_BUILDCGIRES_H_
#include <string>
#include <iostream>
#include <sstream>
#include "common/pbjson/pbjson.h"
#include "common/app/CgiCode.h"
#include "thirdparty/plog/Log.h"

using namespace std;

template<class T>
void SendPostResWithData(int code, const T &data) {
  string json;
  pbjson::pb2json(&data, json);

  ostringstream oss;
  oss << "Content-type: text/script\n\n"
      << "{\"code\":" << code << ","
      << "\"message\":\"" << GetErrMsg(code) << "\","
      << "\"data\":" << json << "}\n";

  LOG_ERROR << "cgi response";
  LOG_ERROR << "------------------------";
  LOG_ERROR << oss.str();

  cout << oss.str();

}

void SendPostResWithoutData(int code) {

  ostringstream oss;
  oss << "Content-type: text/script\n\n"
      << "{\"code\":" << code << ","
      << "\"message\":\"" << GetErrMsg(code)<< "\"}\n";

  LOG_ERROR << "cgi response";
  LOG_ERROR << "------------------------";
  LOG_ERROR << oss.str();

  cout << oss.str();
}

void SendAddressRes(int code, const AddressRes &res) {
  // string json;
  // pbjson::pb2json(&data, json);

  string result;
  ::google::protobuf::Map<::std::string, ::std::string>::const_iterator iter;
  for (iter = res.address().begin(); iter != res.address().end(); ++iter) {
    result += "\"" + iter->first + "\":\"" + iter->second + "\"," ;
  } 
  // erase last ","
  result.pop_back();
  
  ostringstream oss;
  oss << "Content-type: text/script\n\n"
      << "{\"code\":" << code << ","
      << "\"message\":\"" << GetErrMsg(code) << "\","
      // << "\"data\":" << json << "}\n";
      << "\"data\": {" << result << "}}\n";

  LOG_ERROR << "cgi response";
  LOG_ERROR << "------------------------";
  LOG_ERROR << oss.str();

  cout << oss.str();

}

#endif // CGI_LIB_BUILDCGIRES_H_
