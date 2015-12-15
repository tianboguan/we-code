#include <string>
#include "thirdparty/glog/logging.h"
#include "thirdparty/gflags/gflags.h"

void InitLog(const std::string &dir,
                    const std::string &name,                
                    int level,
                    int size);


int main(int argc, char *argv[]) {
  FLAGS_log_dir = "./log";  
  google::InitGoogleLogging(argv[0]); 
  google::ParseCommandLineFlags(&argc, &argv, true);

  cout << FLAGS_A << endl;
  cout << FLAGS_B << endl;
  cout << FLAGS_C << endl;
  // cout << fLI::FLAGS_log_dir << endl;


  //LOG(FATAL) << "FATAL msg";
  LOG(ERROR) << "Error msg";
  LOG(WARNING) << "WARNING msg";
  LOG(INFO) << "INFO msg";
  return 0;
}
#else
//#include "thirdparty/gflags/gflags.h"
#include "thirdparty/glog/logging.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[]) {  
  // If glog is used to parse the command line   
  // google::ParseCommandLineFlags(&argc, &argv, true);  

  // Initialize Google's logging library.  
  cout << argv[0] << endl;
  google::InitGoogleLogging(argv[0]);  

  FLAGS_log_dir = "./log";  

  LOG(INFO) << "hello world";  

  return 0;  
}  
#endif
