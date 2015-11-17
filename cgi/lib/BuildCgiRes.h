#ifndef CGI_LIB_BUILDCGIRES_H_
#define CGI_LIB_BUILDCGIRES_H_
#include <string>
#include <iostream>
#include "common/utils/Pb2Json.h"

using namespace std;

template<class T>
void SendPostResWithData(int code, const string &msg, const T &data) {
  cout << "Content-type: text/script\n\n"
      << "code: " << code << "\n"
      << "message: " << msg << "\n"
      << "data: " << Pb2Json(data) << "\n";
}

void SendPostResWithoutData(int code, const string &msg) {
  cout << "Content-type: text/script\n\n"
      << "code: " << code << "\n"
      << "message: " << msg << "\n";
}


#endif // CGI_LIB_BUILDCGIRES_H_
