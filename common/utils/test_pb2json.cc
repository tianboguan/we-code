#include <iostream>
#include "proto/hello_proto/hello.pb.h"
#include "common/utils/Pb2Json.h"

using namespace std;

int main() {
  HelloMsg hello_msg;
  hello_msg.set_hello("lalala we lalala");
  hello_msg.set_standard_int(1);
  hello_msg.set_long_int(123456789012);
  hello_msg.add_tags("11111");
  hello_msg.add_tags("2222");
  hello_msg.add_tags("3333");

  cout << hello_msg.DebugString() << endl;

  // test pb to string
  string json;
  json = Pb2Json(hello_msg, false);
  cout << json << endl;
  cout << Pb2Json(hello_msg, true) << endl;

  // test string to pb
  HelloMsg hello_msg1;
  Json2Pb(json, &hello_msg1);
  cout << "Json2Pb result: \n----------------" << endl;
  cout << hello_msg.DebugString() << endl;
  return 0;
}
