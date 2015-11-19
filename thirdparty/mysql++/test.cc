
#include "thirdparty/mysql++/mysql++.h"
#include <iostream>

using namespace std;
using namespace mysqlpp;


int main(int argc, char *argv[]) {
  Connection  cnnt ("mysql", "10.66.163.167", "root",
			"guo123!@#", 3306);

  if (cnnt.connected()) {
    cout << "good job" << endl;
  } else {
    cout << "bad job" << endl;
  }

  return 0;
}
