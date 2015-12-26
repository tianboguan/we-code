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
    'Recommand.cc',
  ],
  deps = [
    '//proto:CgiReqProto',
    '//proto:UserProto',
    '//proto:FollowProto',
    '//proto:BusinessProto',
    '//common/redis_utils:RedisUtils',
    '//common/app:AppComm',
    '//common/encode:encode',
    '//common/sms:sms',
    '//common/pbjson:pbjson',
  ]
)
