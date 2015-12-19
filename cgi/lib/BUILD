cc_library(
  name = 'cgi',
  srcs = [
    'ParseCgiReq.cc',
    'Login.cc',
    'Follow.cc',
    'Profile.cc',
    'Business.cc',
    'Interact.cc',
    'Record.cc',
  ],
  deps = [
    '//proto:CgiReqProto',
    '//proto:UserProto',
    '//proto:FollowProto',
    '//common/redis_utils:RedisUtils',
    '//common/app:AppComm',
    '//common/encode:encode',
    '//common/sms:sms',
    '//common/pbjson:pbjson',
    #'//thirdparty/grpc++:grpc++_unsecure',
    #'//thirdparty/grpc:grpc',
    #'//thirdparty/grpc:gpr',
    #'//thirdparty/protobuf:protobuf',   
    #'//thirdparty/glog:glog',
    #'#pthread',
    #'#dl',
    #'#rt'
  ]
)
