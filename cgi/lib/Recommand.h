#ifndef CGI_LIB_RECOMMAND_H_
#define CGI_LIB_RECOMMAND_H_

#include <string>
#include "proto/CgiReq.pb.h"


class Recommand {
  public:
    Recommand(const std::string &user) : user_(user) {}; 
    int Get(const RecommandReq &req, RecommandRes *res);
  private:

  private:
    std::string user_;
};

#endif // CGI_LIB_RECOMMAND_H_
