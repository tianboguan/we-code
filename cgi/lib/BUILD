cc_library(
  name = 'cgi',
  srcs = [
    'ParseCgiReq.cc',
    'Login.cc',
    'Follow.cc',
    'Profile.cc',
    'Business.cc',
    'CgiCode.cc',
    'Interact.cc',
  ],
  deps = [
    '//proto:cgi_req_proto',
    '//proto:cache_data_proto',
    '//common/redis_utils:RedisUtils',
    '//common/encode:encode',
    '//common/sms:sms',
  ]
)
