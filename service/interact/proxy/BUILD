cc_binary(
  name = 'interact_proxy_server',
  srcs = 'main.cc',
  deps = [ 
    '//service/interact/proto:InteractConf',
    '//proto/IPC:InteractIpc',
    '//common/utils:Utils',
    '//thirdparty/grpc++:grpc++_unsecure',
    '//thirdparty/grpc:grpc',
    '//thirdparty/grpc:gpr',
    '//thirdparty/protobuf:protobuf',
    '//thirdparty/glog:glog',
    '#pthread',
    '#dl',
    '#rt'
  ]   
)

