#include <iostream>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/type_resolver.h>
#include <google/protobuf/util/type_resolver_util.h>
#include <google/protobuf/descriptor.h>
#include "proto/hello_proto/hello.pb.h"

using namespace std;
using namespace google::protobuf::util;
using namespace google;
using namespace google::protobuf;

static const char kTypeUrlPrefix[] = "type.googleapis.com";
static string GetTypeUrl(const Descriptor* message) {
  return string(kTypeUrlPrefix) + "/" + message->full_name();
}

int main() {
  HelloMsg hello_msg;
  hello_msg.set_hello("lalala we lalala");
  hello_msg.set_standard_int(1);
  hello_msg.set_long_int(123456789012);
  hello_msg.add_tags("11111");
  hello_msg.add_tags("2222");
  hello_msg.add_tags("3333");

  cout << hello_msg.DebugString() << endl;

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

  cout << endl;
  cout << "json string: "<< endl;
  cout << json_result << endl;

  return 0;
}
