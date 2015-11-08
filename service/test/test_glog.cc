#include "thirdparty/glog/logging.h"
#include "thirdparty/gflags/gflags.h"

int main(int argc, char *argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]); 
  // LOG(FATAL) << "FATAL msg";
  LOG(ERROR) << "Error msg";
  LOG(WARNING) << "WARNING msg";
  LOG(INFO) << "INFO msg";
  return 0;
}
