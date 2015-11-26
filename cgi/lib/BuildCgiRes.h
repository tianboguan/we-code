#ifndef CGI_LIB_BUILDCGIRES_H_
#define CGI_LIB_BUILDCGIRES_H_
#include <string>
#include <iostream>
#include "common/utils/PbJsonUtil.h"
#include "cgi/lib/CgiCode.h"

using namespace std;

template<class T>
void SendPostResWithData(int code, const T &data) {
  PbJsonUtil<T> util;
  string json;
  util.Pb2Json(data, &json);
  cout << "Content-type: text/script\n\n"
      << "code: " << code << "\n"
      << "message: " << GetErrMsg(code) << "\n"
      << "data: " << json << "\n";
}

void SendPostResWithoutData(int code) {
  cout << "Content-type: text/script\n\n"
      << "code: " << code << "\n"
      << "message: " << GetErrMsg(code)<< "\n";
}


#endif // CGI_LIB_BUILDCGIRES_H_
