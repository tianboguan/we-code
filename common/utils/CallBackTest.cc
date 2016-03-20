#include <iostream>
#include <vector>
using namespace std;

template <class T>
bool Print(T t) {
  cout << "------" << t << "------" << endl;
  return true;
}

template <class T>
bool Print2(T t) {
  cout << "++++++" << t << "++++++" << endl;
  return true;
}

typedef bool (*fpt)(int);
bool (*fp)(int);

class CallBack {
  public:
    void register_callback(fpt a) {
      callbacks_.push_back(a);
    }

    void run(int param) {
      vector<fpt>::const_iterator iter;
      for (iter = callbacks_.begin(); iter != callbacks_.end(); ++iter) {
        (*iter)(param);
      }
      return;
    }

  private:
    vector<fpt> callbacks_;

};

int main(int argc, char *argv[]) {
  Print<string>("hello callback");
  Print<int>(1);
  fpt f1;

  // function pointer test
  f1 = Print<int>;
  f1(2);
  fp = Print<int>;
  fp(3);

  // callback test
  CallBack callback;
  callback.register_callback(f1);
  f1 = Print2<int>;
  callback.register_callback(f1);

  callback.run(100);
  return 0;
}
