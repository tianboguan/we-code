#ifndef COMMON_CGI_UTILS_CGIHANDLE_H_
#define COMMON_CGI_UTILS_CGIHANDLE_H_

#include <string>
#include <iterator>
#include <map>
#include "thirdparty/cgicc/Cgicc.h"
#include "thirdparty/cgicc/CgiEnvironment.h"

using namespace std;

class CgiHandle{
  public:
    CgiHandle();
    string operator [] (const string& key);
    string GetMethod();
    const map<string, string> &GetParams();

  private:
    cgicc::Cgicc form_data_;
    map<string, string> query_kv_;
    string method_;
};

#endif // COMMON_CGI_UTILS_CGIHANDLE_H_
