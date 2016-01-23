cc_library(
  name = 'AppComm',
  srcs = [
    'RedisKeys.cc',
    'FollowApi.cc',
    'ProfileApi.cc',
    'InteractApi.cc',
    'StatisticApi.cc',
    'RecordApi.cc',
    'CgiCode.cc',
  ],
  deps = [
    '//proto:FollowProto',
    '//proto:InteractProto',
    '//proto:RecordProto',
    '//proto:ProfileProto',
    '//proto:StatisticProto',
    '//common/redis_utils:RedisUtils',
    '//common/utils:Utils',
  ]
)

cc_test(
  name = 'FollowApi_test',    
  srcs = 'FollowApi_test.cc',
  deps = [
    '//common/app:AppComm',
    '//thirdparty/glog:glog',
  ]
)   

cc_test(
  name = 'RecordApi_test',    
  srcs = 'RecordApi_test.cc',
  deps = [
    '//common/app:AppComm',
    '//thirdparty/glog:glog',
  ]
)   

cc_test(
  name = 'InteractApi_test',    
  srcs = 'InteractApi_test.cc',
  deps = [
    '//common/app:AppComm',
    '//thirdparty/glog:glog',
  ]
)   
