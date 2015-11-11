#include "thirdparty/gflags/gflags.h"
//#include "thirdparty/glog/logging.h"
#include <iostream>

using namespace std;

DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_string(languages, "english,french,german", "comma-separated list of languages to offer in the 'lang' menu");
DEFINE_string(log_dir, "", "log output file");

int main(int argc, char **argv) {
  //google::InitGoogleLogging(argv[0]); 
  google::ParseCommandLineFlags(&argc, &argv, true);

  cout << "argc=" << argc << endl;
  if (FLAGS_big_menu) {
    cout << "big menu is ture" << endl;
  } else {
    cout << "big menu is flase" << endl;
  }

  cout << "languages=" << FLAGS_languages << endl;
  cout << "log_dir=" << FLAGS_log_dir << endl;
  return 0;
}
