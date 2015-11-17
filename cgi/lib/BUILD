cc_library(
  name = 'cgi',
  srcs = [
    'ParseCgiReq.cc',
    'Login.cc',
  ],
  deps = [
    '//proto:cgi_req_proto',
    '//proto:cache_data_proto',
    '//common/redis_utils:RedisUtils',
    '//common/encode:encode',
    '//common/sms:sms',
  ]
)
