#include <iostream>
#include <fstream>
#include "thirdparty/glog/logging.h"
#include "common/cgi_utils/CgiParam.h"

using namespace std;

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]); 
  FLAGS_log_dir = "/data/log/cgi";  
  FLAGS_logtostderr = false;
  FLAGS_stderrthreshold = 3;
  cout << "Content-type: text/script\n\nhello, CGI" << endl;
  ofstream of("/var/www/cgi-bin/log.txt");
  of << "hello.cig" << endl;
  LOG(ERROR) << "glog in cgi test, error"; 
  LOG(WARNING) << "glog in cgi test, warning"; 
  LOG(INFO) << "glog in cgi test, info"; 
  CgiParam cgi_handle;
  cout << "method: " << cgi_handle.GetMethod()<< endl; 
  map<string, string> params = cgi_handle.GetParams();
  map<string, string>::iterator iter;

  for (iter = params.begin(); iter != params.end(); ++iter) {
    cout << iter->first << " --> " << iter->second << endl;
  }

  cout << "key index: p1 -> " << params["p1"] << endl;
  cout << "key index: p4 -> " << params["p4"] << endl;
  
  return 0;
}
