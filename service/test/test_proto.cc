#include <iostream>
#include "proto/hello_proto/hello.pb.h"

using namespace std;

int main(int argc, char *argv[]) {
  HelloMsg hello_msg;
  hello_msg.set_hello("mesage1");
  hello_msg.set_long_int(111111111111);
  cout << "msg1:\n-------------------------------------\n";
  cout << hello_msg.DebugString() << endl;
  HelloMsg hello_msg1;
  hello_msg1.set_hello("message2");
  hello_msg1.set_standard_int(2);
  hello_msg1.set_long_int(222222222222);
  cout << "\n\nmsg2:\n-------------------------------------\n";
  cout << hello_msg1.DebugString() << endl;

  hello_msg1.MergeFrom(hello_msg);
  cout << "\n\nmerger msg:\n-------------------------------------\n";
  cout << hello_msg1.DebugString() << endl;

  return 0;
}
