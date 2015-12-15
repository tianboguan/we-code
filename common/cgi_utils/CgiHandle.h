#ifndef COMMON_CGI_UTILS_CGIHANDLE_H_
#define COMMON_CGI_UTILS_CGIHANDLE_H_

#include <string>
#include <iterator>
#include <map>
#include "thirdparty/cgicc/Cgicc.h"
#include "thirdparty/cgicc/CgiEnvironment.h"

class CgiHandle{
  public:
    CgiHandle();
    std::string operator [] (const std::string& key);
    std::string GetMethod();
    const std::map<std::string, std::string> &GetParams();

  private:
    cgicc::Cgicc form_data_;
    std::map<std::string, std::string> query_kv_;
    std::string method_;
};

#endif // COMMON_CGI_UTILS_CGIHANDLE_H_
