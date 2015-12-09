cc_library(
  name = 'Utils',
  srcs = [
    'FileUtils.cc',
  ],
  deps = [
    '//thirdparty/glog:glog',
  ],
)

cc_binary(
  name = 'FileTest',
  srcs = [
    'FileTest.cc',
  ],
  deps = [
    '//common/utils:Utils',
  ],
)

cc_binary(
  name = 'test_pb2json',
  srcs = 'test_pb2json.cc',
  deps = [
    '//proto/hello_proto:hello_proto',
  ]
)

cc_binary(
  name = 'GLogTest',
  srcs = 'GLogTest.cc',
  deps = [
    '//common/utils:Utils',
  ]
)
