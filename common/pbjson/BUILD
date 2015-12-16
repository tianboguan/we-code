cc_library(
  name = 'pbjson',
  srcs = [
    'pbjson.cc',
  ],
  deps = [
    #'//thirdparty/cgicc:cgicc',
  ]
)

cc_binary(
  name = 'test_pb2json',
  srcs = 'test_pb2json.cc',
  deps = [ 
    '//proto/hello_proto:hello_proto',
    '//common/pbjson:pbjson',
  ]
)
