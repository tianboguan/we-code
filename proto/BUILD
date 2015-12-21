proto_library(
  name = 'CgiReqProto',
  srcs = [ 
    'CgiReq.proto'
  ],  
  deps = [ 
    '//proto:UserProto',
    '//proto:ProfileProto',
    '//proto:RecordProto',
    '//proto:InteractProto'
    #'//common/rpc:qzone_rpc_service_proto',
  ]   
)

proto_library(
  name = 'UserProto',
  srcs = [ 
    'user.proto'
  ],  
  deps = [ 
  ]   
)

proto_library(
  name = 'ProfileProto',
  srcs = [ 
    'profile.proto'
  ],  
  deps = [ 
  ]   
)

proto_library(
  name = 'FollowProto',
  srcs = [ 
    'follow.proto'
  ],  
  deps = [ 
  ]   
)

proto_library(
  name = 'BusinessProto',
  srcs = [ 
    'business.proto'
  ],  
  deps = [ 
  ]   
)

proto_library(
  name = 'InteractProto',
  srcs = [ 
    'interact.proto'
  ],  
  deps = [ 
    #'//proto:ProfileProto',
    '//proto:RecordProto',
    #'//thirdparty/protobuf:protobuf',   
  ]   
)

proto_library(
  name = 'RecordProto',
  srcs = [ 
    'record.proto'
  ],  
  deps = [ 
    '//proto:ProfileProto',
    '//proto:StatisticProto',
    '//thirdparty/grpc++:grpc++_unsecure',
    '//thirdparty/grpc:grpc',
    '//thirdparty/grpc:gpr',
    '//thirdparty/protobuf:protobuf',   
    #'//thirdparty/glog:glog',
    #'#pthread',
    #'#dl',
    '#rt'
  ]   
)

proto_library(
  name = 'StatisticProto',
  srcs = [ 
    'statistic.proto'
  ],  
  deps = [ 
    '//thirdparty/grpc++:grpc++_unsecure',
    '//thirdparty/grpc:grpc',
    '//thirdparty/grpc:gpr',
    #'//thirdparty/protobuf:protobuf',   
    #'//thirdparty/glog:glog',
    #'#pthread',
    #'#dl',
    '#rt',
  ]   
)
