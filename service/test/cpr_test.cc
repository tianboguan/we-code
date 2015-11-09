#include "thirdparty/cpr/include/cpr.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
  auto r = cpr::Get(cpr::Url{"https://github.com/tianboguan"},
      cpr::Authentication{"122652151@qq.com", "guo123!@#"},
      cpr::Parameters{{"anon", "true"}, {"key", "value"}});
  cout << r.status_code << endl;                  // 200
  // cout << r.headers["content-type"] << endl;      // application/json; charset=utf-8
  cout << r.header["content-type"] << endl;      // application/json; charset=utf-8
  cout << r.text << endl;                         // JSON text string
}
