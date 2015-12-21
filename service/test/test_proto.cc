#include <iostream>
#include "proto/business.pb.h"
#include "common/utils/PbUtils.h"
#include "common/utils/StringUtils.h"

// #include "common/utils/GLogWrapper.h"

using namespace std;

int main(int argc, char *argv[]) {
#if 0
  TagElement tag;
  tag.set_name("gogo");
  tag.set_url("http://gogo");
#endif
#if 0
  message AddressNode {
    string code = 1;
    string name = 2;
  }

  message AddressSetting {
    AddressNode father = 1;
    repeated AddressNode childs = 2;
  }

  message AddressConf {
    map<string, AddressSetting> code2addr = 1;

#endif
#if 1
#if 0
    cout << "\n-------- build result ----------\n" << addr.Utf8DebugString() << endl;
    if (DumpPbConfigToFile("./dump_result", addr) != 0) {
      cout << "dump config to file error" << endl;
    } else {
      cout << "dump config to file success" << endl;
    }
#endif
    AddressConf addr2;
    //int ret = ParsePbConfigFromFile("./address.conf", &addr2);
    int ret = ParsePbConfigFromFile("/home/joshua/project/we-code/build64_release/cgi/address.conf", &addr2);
    // int ret = ParsePbConfigFromFile("./dump_result", &addr2);
    if (ret != 0) {
      cout << "xxxxxxxxxxxxxxx" << endl;
    } else {
      cout << "\n-------- parse result ----------\n" << addr2.Utf8DebugString() << endl;
      // string result;
      // google::protobuf::TextFormat::PrintToString(addr2, &result);
      if (DumpPbConfigToFile("./format_addr.conf", addr2) != 0) {
        cout << "dump addr to file error" << endl;
      } else {
        cout << "dump addr to file success" << endl;
      }
    }
#else
    string result;
    google::protobuf::TextFormat::PrintToString(addr, &result);

    cout << "result:\n" << result << endl;
#endif

    return 0;
  }
