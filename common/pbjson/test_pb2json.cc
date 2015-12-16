#include <iostream>
#include "proto/hello_proto/hello.pb.h"
#include "common/pbjson/pbjson.h"

using namespace std;

using namespace pbjson;

int main() {
  HelloMsg hello_msg;
  hello_msg.set_hello("操气喘吁吁 ");
  // hello_msg.set_hello("11111111111");
  hello_msg.set_standard_int(1);
  hello_msg.set_long_int(123456789012);
  hello_msg.add_tags("11111");
  hello_msg.add_tags("2222");
  hello_msg.add_tags("3333");

  cout << hello_msg.Utf8DebugString() << endl;

  // test pb to string
  string json1;
  pb2json(&hello_msg, json1);
  cout << "pb2json:\n" << json1 << endl;

  string json = "{\"hello\": \"你\", \"standard_int\": 22}";

  // test string to pb
  HelloMsg hello_msg1;
  string err;
  HelloMsg hell;
  json2pb(json, &hell, err);
  cout << "new Json2Pb result: \n----------------" << endl;
  cout << hell.Utf8DebugString() << endl;
  cout << hell.DebugString() << endl;

  string json2;
  pb2json(&hell, json2);
  cout << "hahaha: \n" << json2  << endl;

  return 0;
}
