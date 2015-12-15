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
  name = 'GLogTest',
  srcs = 'GLogTest.cc',
  deps = [
    '//common/utils:Utils',
  ]
)
