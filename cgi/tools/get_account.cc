#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"
#include "common/utils/Pb2Json.h"

#include "thirdparty/gflags/gflags.h"
#include <iostream>

using namespace std;

DEFINE_string(user, "18127813634", "user phone number");
int main(int argc, char *argv[]) {
  plog::init(plog::debug, "./read_account.log");
  google::ParseCommandLineFlags(&argc, &argv, true);

  cout << "FLAGS_user: " << FLAGS_user << endl; 
  Account account(FLAGS_user, ""); 
  AccountInfo account_info;
  int ret = account.Read(account_info);
  if (ret != 0) {
    cout << "read user info failed" << endl;
    return -1;
  }

  cout << account_info.DebugString() << endl;

  return 0;
}
