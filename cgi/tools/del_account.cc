#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "cgi/lib/Login.h"
#include "thirdparty/gflags/gflags.h"
#include <iostream>

using namespace std;

DEFINE_string(user, "18127813634", "user phone number");
int main(int argc, char *argv[]) {
  plog::init(plog::debug, "./del_account.log");
  google::ParseCommandLineFlags(&argc, &argv, true);

  Account account(FLAGS_user, ""); 
  int ret = account.Del();
  if (ret != 0) {
    cout << "del user info failed" << endl;
    return -1;
  }
  return 0;
}
