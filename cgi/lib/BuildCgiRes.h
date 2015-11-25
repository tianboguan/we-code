#ifndef CGI_LIB_BUILDCGIRES_H_
#define CGI_LIB_BUILDCGIRES_H_
#include <string>
#include <iostream>
#include "common/utils/Pb2Json.h"
#include "cgi/lib/CgiCode.h"

using namespace std;

template<class T>
void SendPostResWithData(int code, const T &data) {
  cout << "Content-type: text/script\n\n"
      << "code: " << code << "\n"
      << "message: " << GetErrMsg(code) << "\n"
      << "data: " << Pb2Json(data) << "\n";
}

void SendPostResWithoutData(int code) {
  cout << "Content-type: text/script\n\n"
      << "code: " << code << "\n"
      << "message: " << GetErrMsg(code)<< "\n";
}


#endif // CGI_LIB_BUILDCGIRES_H_
