cc_library(
  name = 'cgi',
  srcs = [
    'ParseCgiReq.cc',
    'Login.cc',
    'Follow.cc',
    'Profile.cc',
    'Business.cc',
    #'CgiCode.cc',
    #'Interact.cc',
    #'Record.cc',
  ],
  deps = [
    '//proto:CgiReqProto',
    #'//proto:cache_data_proto',
    '//proto:UserProto',
    '//proto:FollowProto',
    '//common/redis_utils:RedisUtils',
    '//common/app:AppComm',
    '//common/encode:encode',
    '//common/sms:sms',
    '//common/pbjson:pbjson',
  ]
)
