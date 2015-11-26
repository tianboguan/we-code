#include <iostream>
#include "proto/hello_proto/hello.pb.h"
#include "common/utils/PbJsonUtil.h"

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

  PbJsonUtil<HelloMsg> util;
  bool ret;
  // test pb to string
  string json;
  ret = util.Pb2Json(hello_msg, &json);
  cout << json << endl;

  // test string to pb
  HelloMsg hello_msg1;
  ret = util.Json2Pb(json, &hello_msg1);
  cout << "Json2Pb result: \n----------------" << endl;
  cout << hello_msg1.DebugString() << endl;

  //json += "wrong xxxx";
  json = "wrong xxxx";
  ret = util.Json2Pb(json, &hello_msg1);
  if (ret) {
    cout << "Json2Pb result 222222222222222: \n----------------" << endl;
    cout << hello_msg1.DebugString() << endl;
  } else {
    cout << "error: " << util.error() << endl;
  }
  return 0;
}
