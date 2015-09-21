cc_binary(
  name = 'hello',
  srcs = 'main.cc',
  deps = [
    '//common:hello_blade',
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
