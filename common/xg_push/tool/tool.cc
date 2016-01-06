#include <iostream>
#include "common/xg_push/XGPush.h"

using namespace std;

int main(int argc, char *argv[]) {
  XGPush xg_push;
  bool ret;
  if (argc < 3) {
    ret = xg_push.PushSingleAccount("28", "hello xg");
  } else {
    ret = xg_push.PushSingleAccount(argv[1], argv[2]);
  }
  if (ret) {
    cout << "xg push success!" << endl;
  } else {
    cout << "xg push failed! \n" << xg_push.ErrorMsg() << endl;
  }
  return 0;
}
