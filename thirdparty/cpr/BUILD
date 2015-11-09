cc_library(
  name = "cpr",
  srcs = [ 
    "src/auth.cpp",
    "src/cookies.cpp",
    "src/cprtypes.cpp",
    "src/digest.cpp",
    "src/multipart.cpp",
    "src/parameters.cpp",
    "src/payload.cpp",
    "src/proxies.cpp",
    "src/session.cpp",
    "src/util.cpp"
  ],  
  incs = './include/',
  deps = [ 
    '#curl',
  ],  
  warning = 'no'
)

cc_test(
  name = 'get_tests',
  srcs = [
    'test/get_tests.cpp',
    'test/server.cpp',
  ],
  incs = './include/',
  deps = [
    ':cpr',
  ],
)
