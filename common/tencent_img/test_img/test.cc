#include <iostream>
#include "common/tencent_img/TencentImg.h"

using namespace std;

int main(int argc, char *argv[]) {
  TencentImg tencent_img;
  if (argc < 2) {
    cout << tencent_img.GetPublicSign() << endl;
  } else {
    cout << tencent_img.GetPrivateSign(argv[1]) << endl;
  }
  return 0;
}
