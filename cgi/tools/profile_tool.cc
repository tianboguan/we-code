#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/app/ProfileApi.h"
#include "common/app/CgiCode.h"
#include "thirdparty/gflags/gflags.h"
#include <iostream>
#include "common/redis_utils/RedisPb.h"
#include "common/app/RedisKeys.h"
#include "proto/profile.pb.h"
#include "common/redis_utils/RedisCpp.h"
#include "common/redis_utils/RedisPb.h"
#include "common/utils/PbJsonUtil.h"

using namespace std;

std::string hex_encode(const std::string& input)
{
  static const char* const lut = "0123456789abcdef";
  size_t len = input.length();

  std::string output;
  output.reserve(2 * len);
  for (size_t i = 0; i < len; ++i)
  {
    const unsigned char c = input[i];
    output.push_back(lut[c >> 4]);
    output.push_back(lut[c & 15]);
  }
  return output;
}
char * pb2json(const google::protobuf::Message &msg)
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  json_t *root = parse_msg(&msg);
  char *json = json_dumps(root,0);
  json_decref(root);
  return json; // should be freed by caller
}
char * pb2json( google::protobuf::Message *msg,const char *buf ,int len)
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  std::string s (buf,len);
  msg->ParseFromString(s);
  json_t *root = parse_msg(msg);
  char *json = json_dumps(root,0);
  json_decref(root);
  return json; // should be freed by caller
}
static json_t *parse_repeated_field(const google::protobuf::Message *msg,const google::protobuf::Reflection * ref,const google::protobuf::FieldDescriptor *field)
{
  size_t count = ref->FieldSize(*msg,field);
  json_t *arr = json_array(); 
  if(!arr)return NULL;
  switch(field->cpp_type()) 
  {
    case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
      for(size_t i = 0 ; i != count ; ++i)
      {
        double  value1 = ref->GetRepeatedDouble(*msg,field,i);
        json_array_append_new(arr,json_real(value1)); 
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
      for(size_t i = 0 ; i != count ; ++i)
      {
        float value2 = ref->GetRepeatedFloat(*msg,field,i);
        json_array_append_new(arr,json_real(value2)); 
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
      for(size_t i = 0 ; i != count ; ++i)
      {
        int64_t value3 = ref->GetRepeatedInt64(*msg,field,i);
        json_array_append_new(arr,json_integer(value3)) ;
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
      for(size_t i = 0 ; i != count ; ++i)
      {
        uint64_t value4 = ref->GetRepeatedUInt64(*msg,field,i);
        json_array_append_new(arr,json_integer(value4));  
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
      for(size_t i = 0 ; i != count ; ++i)
      {
        int32_t value5 = ref->GetRepeatedInt32(*msg,field,i);
        json_array_append_new(arr,json_integer(value5));  
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
      for(size_t i = 0 ; i != count ; ++i)
      {
        uint32_t value6 = ref->GetRepeatedUInt32(*msg,field,i);
        json_array_append_new(arr,json_integer(value6));  
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
      for(size_t i = 0 ; i != count ; ++i)
      {
        bool value7 = ref->GetRepeatedBool(*msg,field,i);
        json_array_append_new(arr,value7?json_true():json_false()) ;
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
      for(size_t i = 0 ; i != count ; ++i)
      {
        std::string value8 = ref->GetRepeatedString(*msg,field,i);
        json_array_append_new(arr,json_string(value8.c_str())); 
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
      for(size_t i = 0 ; i != count ; ++i)
      {
        const google::protobuf::Message *value9 = &(ref->GetRepeatedMessage(*msg,field,i));
        json_array_append_new(arr,parse_msg(value9));
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
      for(size_t i = 0 ; i != count ; ++i)
      {
        const google::protobuf::EnumValueDescriptor* value10 = ref->GetRepeatedEnum(*msg,field,i);
        json_array_append_new(arr,json_integer(value10->number())); 
      }
      break;
    default:
      break;
  }
  return arr;
}
static json_t *parse_msg(const google::protobuf::Message *msg)
{
  const google::protobuf::Descriptor *d = msg->GetDescriptor();
  if(!d)return NULL;
  size_t count = d->field_count();
  json_t *root = json_object();
  if(!root)return NULL;
  for (size_t i = 0; i != count ; ++i)
  {
    const google::protobuf::FieldDescriptor *field = d->field(i);
    if(!field)return NULL;

    const google::protobuf::Reflection *ref = msg->GetReflection();
    if(!ref)return NULL;
    const char *name = field->name().c_str();
    if(field->is_repeated())
      json_object_set_new(root,name,parse_repeated_field(msg,ref,field));
    if(!field->is_repeated() && ref->HasField(*msg,field))
    {

      double value1;
      float value2;
      int64_t value3;
      uint64_t value4;
      int32_t value5;
      uint32_t value6;
      bool value7;
      std::string value8;
      const google::protobuf::Message *value9;
      const google::protobuf::EnumValueDescriptor *value10;

      switch (field->cpp_type())
      {
        case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
          value1 = ref->GetDouble(*msg,field);
          json_object_set_new(root,name,json_real(value1)); 
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
          value2 = ref->GetFloat(*msg,field);
          json_object_set_new(root,name,json_real(value2)); 
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
          value3 = ref->GetInt64(*msg,field);
          json_object_set_new(root,name,json_integer(value3)) ;
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
          value4 = ref->GetUInt64(*msg,field);
          json_object_set_new(root,name,json_integer(value4));  
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
          value5 = ref->GetInt32(*msg,field);
          json_object_set_new(root,name,json_integer(value5));  
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
          value6 = ref->GetUInt32(*msg,field);
          json_object_set_new(root,name,json_integer(value6));  
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
          value7 = ref->GetBool(*msg,field);
          json_object_set_new(root,name,value7?json_true():json_false())  ;
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
          if (field->type() == google::protobuf::FieldDescriptor::TYPE_BYTES) {
            value8 = hex_encode(ref->GetString(*msg,field));
          } else {
            value8 = ref->GetString(*msg,field);
          }
          json_object_set_new(root,name,json_string(value8.c_str()));  
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
          value9 = &(ref->GetMessage(*msg,field));
          json_object_set_new(root,name,parse_msg(value9));
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
          value10 = ref->GetEnum(*msg,field);
          json_object_set_new(root,name,json_integer(value10->number())); 
          break;
        default:
          break;
      }

    }

  }
  return root;

}
DEFINE_string(user, "", "user");
DEFINE_string(action, "", "get or del");
DEFINE_string(type, "", "base or not");
DEFINE_string(json, "", "yes or not");

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "./profile_tool.log");
  ::google::ParseCommandLineFlags(&argc, &argv, true);
  cout << "--------------input-------------" << endl;
  cout << "user: " << FLAGS_user << endl;
  cout << "action: " << FLAGS_action << endl;
  cout << "type: " << FLAGS_action << endl;
  cout << "\n----------------result------------" << endl;

  ProfileApi Api;
  if (FLAGS_action == "get") {
    std::string result;
    if (FLAGS_type == "base") {
      UserBaseInfo info;
      if (Api.Get(FLAGS_user, &info) != kCgiCodeOk) {
        cout << "error, user: " << FLAGS_user << endl;
      } else {
        if (FLAGS_json == "yes") {
          PbJsonUtil<UserBaseInfo> utils;
          utils.Pb2Json(info, &result);

        } else {
          result = info.Utf8DebugString();
        }
      }
    } else {
      UserProfile profile;
      if (Api.Get(FLAGS_user, &profile) != kCgiCodeOk) {
        cout << "error, user: " << FLAGS_user << endl;
      } else {
        if (FLAGS_json == "yes") {
          PbJsonUtil<UserProfile> utils;
          utils.Pb2Json(profile, &result);
          cout << "name: " << profile.nickname() << endl;
        } else {
          result = profile.Utf8DebugString();
        }
      }
    }

    cout << result << endl; 

  } else if (FLAGS_action == "del") {
      if (Api.Del(FLAGS_user) != kCgiCodeOk) {
        cout << "error, user: " << FLAGS_user << endl;
      } else {
        cout << "error, user: " << FLAGS_user << endl;
      }
  } else {
    cout << "invalid action type, option: get | del" << endl;
  }

  return 0;
}
