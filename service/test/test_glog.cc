#include "thirdparty/glog/logging.h"
#include "thirdparty/gflags/gflags.h"
#include <iostream>
using namespace std;

#if 1

void init_log(const char* haha) {
  FLAGS_log_dir = "./log";  
  google::InitGoogleLogging(haha); 
}
#endif


int main(int argc, char *argv[]) {
#if 0
  FLAGS_log_dir = "./log";  
  google::InitGoogleLogging(argv[0]); 
  //google::InitGoogleLogging("kao"); 
  //google::ParseCommandLineFlags(&argc, &argv, true);
#else
  init_log(argv[0]);
#endif


  //LOG(FATAL) << "FATAL msg";
  LOG(ERROR) << "Error msg";
  LOG(WARNING) << "WARNING msg";
  LOG(INFO) << "INFO msg";
  return 0;
}
