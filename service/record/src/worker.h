
#ifndef SERVICE_RECORD_SRC_WORKER_H_
#define SERVICE_RECORD_SRC_WORKER_H_

#include <memory>
#include "proto/record.pb.h"
#include "service/record/proto/RecordConf.pb.h"
#include "common/msgque/SupperQueue.h"
#include "common/app/RecordApi.h"

class RecordWorker {
  public:
    RecordWorker(const RecordQueueConf &conf) : conf_(conf){}
    bool Init();
    void Run();

  private:
    bool Handle(const RecordReq &req);
    bool Add(const std::string &id);
    bool LinkToHome(const RoughRecord &record);
    bool LinkToActive(const RoughRecord &record);
    bool LinkToRecent(const RoughRecord &record);

  private:
    RecordQueueConf conf_;
    std::unique_ptr<imque::SupperQueue<RecordReq> > queue_;
    RecordApi api_;
};

#endif  // SERVICE_RECORD_SRC_WORKER_H_
