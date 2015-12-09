#include <iostream>
#include <string>

using namespace std;

#include "common/msgque/SupperQueue.h"
#include "proto/hello_proto/hello.pb.h"

static const int QUEUE_SHM_SIZE = 512; 

int main(int argc, char** argv) {

  const char* shm_file_path = "./queue_shm";
  imque::SupperQueue<HelloMsg> que(QUEUE_SHM_SIZE, shm_file_path);
  if(! que) {
    std::cerr << "queue initialization failed: " << shm_file_path << std::endl;
    return 1;
  }

  if (que.isEmpty()) {
    cout << "que is empty" << endl;
  } else {
    cout << "overflow: " << que.overflowedCount() << endl; 
  }

  int i = 0;
  while(true) {
    HelloMsg hello_msg;
    hello_msg.set_hello("lalala we lalala");
    hello_msg.set_standard_int(1);
    hello_msg.set_long_int(123456789012);
    hello_msg.add_tags("11111");
    hello_msg.add_tags("2222");
    hello_msg.add_tags("3333");

    if (!que.enq(hello_msg)) {
      cout << "enq msg failed! " << endl;
      break;
    } else {
      cout << "enq msg success! index: " << ++i << endl;
    }
  }

  cout << "\nafter enq ----------------------" << endl;

  if (que.isEmpty()) {
    cout << "que is empty";
  } else {
    cout << "overflow: " << que.overflowedCount() << endl; 
  }

  int j = 0;
  while(true) {
    HelloMsg msg;
    if (que.deq(&msg)) {
      cout << "----------- index: " << ++j << ", total: " << i << "---------------------" << endl;
      cout << msg.DebugString() << endl;
    } else {
      break;
    }
  }

  return 0;
}
