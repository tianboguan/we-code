cc_binary(
  name = 'hello',
  srcs = 'main.cc',
  deps = [
    '//common/hello_common:hello_blade',
  ]
)

cc_binary(
  name = 'test_proto',
  srcs = 'test_proto.cc',
  deps = [
    '//proto/hello_proto:hello_proto',
  ]
)

cc_binary(
  name = 'rpc_client',
  srcs = 'greeter_client.cc',
  deps = [
    '//proto/test_rpc:helloworld',
    '//thirdparty/grpc++:grpc++_unsecure',
    '//thirdparty/grpc:grpc',
    '//thirdparty/grpc:gpr',
    '//thirdparty/protobuf:protobuf',
    '#pthread',
    '#dl',
    '#rt'
  ]
)

cc_binary(
  name = 'rpc_server',
  srcs = 'greeter_server.cc',
  deps = [ 
    '//proto/test_rpc:helloworld',
    '//thirdparty/grpc++:grpc++_unsecure',
    '//thirdparty/grpc:grpc',
    '//thirdparty/grpc:gpr',
    '//thirdparty/protobuf:protobuf',
    '#pthread',
    '#dl',
    '#rt'
  ]   
)

cc_binary(
  name = 'test_rapid_json',
  srcs = 'test_rapid_json.cc',
  #deps = [
  #  '//proto/hello_proto:hello_proto',
  #]
  incs = '../common/rapidjson/',
)

cc_binary(
  name = 'test_pb2json',
  srcs = 'test_pb2json.cc',
  deps = [
    '//proto/hello_proto:hello_proto',
  ]
)

cc_binary(
  name = 'test_glog',
  srcs = 'test_glog.cc',
  deps = [
    '//thirdparty/glog:glog',
  ]
)

cc_binary(
  name = 'test_flags',
  srcs = 'test_flag.cc',
  deps = [
    '//thirdparty/gflags:gflags',
    #'//thirdparty/glog:glog'
  ]
)

cc_binary(
  name = 'cpr_test',
  srcs = 'cpr_test.cc',
  deps = [
    '//thirdparty/cpr:cpr',
  ]
)

cc_binary(
  name = 'redis_test',
  srcs = 'redis_test.cc',
  deps = [
    '//thirdparty/hiredis-master:redis',
  ]
)
