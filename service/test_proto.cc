#include <iostream>
#include "proto/hello_proto/hello.pb.h"

using namespace std;

int main(int argc, char *argv[]) {
  HelloMsg hello_msg;
  hello_msg.set_hello("lalala we lalala");
  hello_msg.set_standard_int(1);
  hello_msg.set_long_int(123456789012);
  cout << hello_msg.DebugString() << endl;
  return 0;
}
