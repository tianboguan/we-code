#ifndef COMMON_UTILS_PB2JSON_H_
#define COMMON_UTILS_PB2JSON_H_

#include <string>
#include <exception>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/type_resolver.h>
#include <google/protobuf/util/type_resolver_util.h>
#include <google/protobuf/descriptor.h>

using namespace google::protobuf::util;
using namespace google;
using namespace google::protobuf;

static std::string kTypeUrlPrefix = "type.googleapis.com";
inline std::string GetTypeUrl(const Descriptor* message) {
  return kTypeUrlPrefix + "/" + message->full_name();
}

template<class T>
class PbJsonUtil {
  public:
    PbJsonUtil(bool strip = false) : strip_(strip) {}

    bool Pb2Json(const T &pb, std::string *json) {
      JsonOptions opts;
      opts.add_whitespace = strip_;
      opts.always_print_primitive_fields = true;

      google::protobuf::scoped_ptr<TypeResolver> resolver_;
      resolver_.reset(NewTypeResolverForDescriptorPool(
            kTypeUrlPrefix, DescriptorPool::generated_pool()));

      status_ = BinaryToJsonString(resolver_.get(), GetTypeUrl(pb.GetDescriptor()),
          pb.SerializeAsString(), json, opts);
      return status_.ok();
    }

    bool Json2Pb(const std::string &json, T *pb) {
      std::string temp;
      google::protobuf::scoped_ptr<TypeResolver> resolver_;
      resolver_.reset(NewTypeResolverForDescriptorPool(
            kTypeUrlPrefix, DescriptorPool::generated_pool()));
      status_ = JsonToBinaryString(resolver_.get(), GetTypeUrl(pb->GetDescriptor()),
          json, &temp);
      if (!status_.ok()) {
        return false;
      }

      return pb->ParseFromString(temp);
    }

    std::string error() {
      return status_.error_message();
    }

  private:
    util::Status status_;
    bool strip_;
};

#endif  // COMMON_UTILS_PB2JSON_H_
