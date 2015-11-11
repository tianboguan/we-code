#if 1
#include "thirdparty/glog/logging.h"
#include "thirdparty/gflags/gflags.h"
#include <iostream>
using namespace std;

DEFINE_int32(B, 0, "Log messages at or above this level. Again, the numbers of severity levels INFO, WARNING, ERROR, and FATAL are 0, 1, 2, and 3, respectively.");
DEFINE_bool(C, false, "Log messages to stderr instead of logfiles.");
DEFINE_string(A, "", "If specified, logfiles are written into this directory instead of the default logging directory.");


int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]); 
  google::ParseCommandLineFlags(&argc, &argv, true);

  cout << FLAGS_A << endl;
  cout << FLAGS_B << endl;
  cout << FLAGS_C << endl;
  // cout << fLI::FLAGS_log_dir << endl;

  FLAGS_log_dir = "./log";  

  //LOG(FATAL) << "FATAL msg";
  LOG(ERROR) << "Error msg";
  LOG(WARNING) << "WARNING msg";
  LOG(INFO) << "INFO msg";
  return 0;
}
#else
//#include "thirdparty/gflags/gflags.h"
#include "thirdparty/glog/logging.h"
int main(int argc,char* argv[]) {  
  // If glog is used to parse the command line   
  // google::ParseCommandLineFlags(&argc, &argv, true);  

  // Initialize Google's logging library.  
  google::InitGoogleLogging(argv[0]);  

  FLAGS_log_dir = "./log";  

  LOG(INFO) << "hello world";  

  return 0;  
}  
#endif
