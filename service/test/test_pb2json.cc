#include <iostream>
#if 0
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/type_resolver.h>
#include <google/protobuf/util/type_resolver_util.h>
#include <google/protobuf/descriptor.h>
#endif
#include "proto/hello_proto/hello.pb.h"
#include "common/utils/Pb2Json.h"

int main() {
  HelloMsg hello_msg;
  hello_msg.set_hello("lalala we lalala");
  hello_msg.set_standard_int(1);
  hello_msg.set_long_int(123456789012);
  hello_msg.add_tags("11111");
  hello_msg.add_tags("2222");
  hello_msg.add_tags("3333");

  cout << hello_msg.DebugString() << endl;

#if 0
  JsonOptions opts;
  opts.add_whitespace = true;
  opts.always_print_primitive_fields = true;

  google::protobuf::scoped_ptr<TypeResolver> resolver_;
  resolver_.reset(NewTypeResolverForDescriptorPool(
        kTypeUrlPrefix, DescriptorPool::generated_pool()));

  string json_result;
  BinaryToJsonString(resolver_.get(),
      GetTypeUrl(hello_msg.GetDescriptor()),
      hello_msg.SerializeAsString(), &json_result, opts);

  cout << "pb debug string: "<< endl;
  cout << hello_msg.DebugString() << endl;
#else
  string json;
  string json_strip;
  PB2String(hello_msg, json, false);
  PB2String(hello_msg, json_strip, true);
#endif

  cout << endl;
  cout << "json : "<< endl;
  cout << json << endl;
  cout << endl;
  cout << endl;
  cout << endl;
  cout << "json strip: "<< endl;
  cout << json_strip << endl;

  return 0;
}
