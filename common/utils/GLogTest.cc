#include "common/utils/GLogWrapper.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
  // InitGlog("log_test", "./log");
  // InitGlog(argv[0], "./log");
  InitGlog("mylog", "./log");
  cout << "alalalal" << endl;

  //LOG(FATAL) << "FATAL msg";
  LOG(ERROR) << "Error msg";
  LOG(WARNING) << "WARNING msg";
  LOG(INFO) << "INFO msg";
  return 0;
}
