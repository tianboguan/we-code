#include <iostream>
#include "common/utils/FileUtils.h"

using namespace std;

int main(int argc, char *argv[]) {

  if (!FileUtils::Exists("file.txt")) {
    cout << "file.txt not exist" << endl;
  }


  if (!FileUtils::WriteStringToFile("hello file", "file.txt")) {
    cout << "write content to file failed!" << endl;
    return 0;
  }
  cout << "file created now" << endl;

  if (!FileUtils::Exists("file.txt")) {
    cout << "file.txt not exist" << endl;
    return 0;
  }
  cout << "file exist now" << endl;


  string result;
  if (!FileUtils::ReadFileToString("file.txt", &result)) {
    cout << "read file failed" << endl;
    return 0;
  }
  cout << "read result: " << result << endl;


  FileUtils::DeleteRecursively("file.txt", NULL, NULL);

  return 0;
}
