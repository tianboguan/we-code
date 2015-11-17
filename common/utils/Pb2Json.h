#ifndef COMMON_UTILS_PB2JSON_H_
#define COMMON_UTILS_PB2JSON_H_

#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/type_resolver.h>
#include <google/protobuf/util/type_resolver_util.h>
#include <google/protobuf/descriptor.h>

using namespace std;
using namespace google::protobuf::util;
using namespace google;
using namespace google::protobuf;

static string kTypeUrlPrefix = "type.googleapis.com";
inline string GetTypeUrl(const Descriptor* message) {
  return kTypeUrlPrefix + "/" + message->full_name();
}

template<class T>
string Pb2Json(const T &pb, bool strip = false) {
  JsonOptions opts;
  opts.add_whitespace = strip;
  opts.always_print_primitive_fields = true;

  string json_result;
  google::protobuf::scoped_ptr<TypeResolver> resolver_;
  resolver_.reset(NewTypeResolverForDescriptorPool(
        kTypeUrlPrefix, DescriptorPool::generated_pool()));

  BinaryToJsonString(resolver_.get(), GetTypeUrl(pb.GetDescriptor()),
                     pb.SerializeAsString(), &json_result, opts);

  return json_result;
}

#endif  // COMMON_UTILS_PB2JSON_H_
