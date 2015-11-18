#include "thirdparty/plog/Log.h"
#include <iostream>
using namespace std;

void test() {
  LOGE << "Hello error log!"; // short macro
  LOG_ERROR<< "Hello error log!"; // long macro
  LOG(plog::error) << "Hello error log!"; // function-style macro
}

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "Hello.txt"); // Step2: initialize the logger.

  // Step3: write log messages using a special macro. 
  // There are several log macros, use the macro you liked the most.
  test();

  LOGD << "Hello log!"; // short macro
  LOG_DEBUG << "Hello log!"; // long macro
  LOG(plog::debug) << "Hello log!"; // function-style macro



  return 0;
}
