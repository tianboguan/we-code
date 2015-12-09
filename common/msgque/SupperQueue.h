#ifndef COMMON_MSGQUE_SUPPERQUEUE_H_
#define COMMON_MSGQUE_SUPPERQUEUE_H_

#include "common/msgque/queue.hh"
#include <string>
#include <vector>
#include <sys/types.h>

namespace imque {
template<class T>
class SupperQueue: public Queue {
  public:
    SupperQueue(size_t shm_size) : Queue(shm_size) {}
    SupperQueue(size_t shm_size, const std::string& filepath, mode_t mode=0660) :
      Queue(shm_size, filepath, mode) {}

    bool enqv(const std::vector<T> &msgs) {
      // TODO
      // bool enqv(const void** datav, size_t* sizev, size_t count)
      return true;
    }

    bool enq(const T &msg) {
      std::string v;
      msg.SerializeToString(&v);
      return Queue::enq(v.data(), v.size());
    }

    bool deq(T *msg) {
      std::string v;
      return Queue::deq(v) ? msg->ParseFromString(v) : false;
    }
};
}

#endif  // COMMON_MSGQUE_SUPPERQUEUE_H_
