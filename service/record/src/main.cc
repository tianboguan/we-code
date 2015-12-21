#include <iostream>
#include <memory>
#include <unistd.h>
#include <string.h>
#include "common/utils/PbUtils.h"
#include "common/utils/StringUtils.h"
#include "common/utils/GLogWrapper.h"
#include "service/record/proto/RecordConf.pb.h"
#include "grpc++/grpc++.h"
#include "common/msgque/SupperQueue.h"
#include "service/record/src/worker.h"
#include "proto/record.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using namespace std;

class RecordService final : public RecordServer::Service  {
  public:
    RecordService(const RecordQueueConf &conf) : conf_(conf) {}

    bool Init() {
      std::unique_ptr<imque::SupperQueue<RecordReq> > queue(
          new imque::SupperQueue<RecordReq>(conf_.shm_size(), conf_.queue_path()));
      queue_ = std::move(queue);
      return queue_.get() != NULL;
    }

    Status HandleRecord(ServerContext* context, const RecordReq* req,
        RecordRes *res) {
      bool  ret;
      ret = queue_->enq(*req);
      LOG_IF(ERROR, !ret) << "push msg to queue failed!";
      if (ret) {
        res->set_code(0);
      } else {
        res->set_code(-1);
      }
      res->set_id(req->id());
      return Status::OK;
    }

  private:
    RecordQueueConf conf_;
    std::unique_ptr<imque::SupperQueue<RecordReq> > queue_;
};

void RecordProxy (const RecordConf &conf) {
  string server_address(conf.host() + value_to_string(conf.port()));
  RecordService service(conf.queue());
  if (!service.Init()) {
    LOG(ERROR) << "init record service failed!";
    return;
  }

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();

  return;
}

void Worker(const RecordConf &conf) {
  RecordWorker worker(conf.queue());
  if (!worker.Init()) {
    LOG(ERROR) << "record request handler task init failed!";
    return;
  }
  worker.Run();
}

int main(int argc, char** argv) {
  if(!InitGlog(argv[0], "../log")) {
    std::cout << "task init failed! name: " << argv[0] << std::endl;
    return -1;
  }

  RecordConf conf;
  CHECK(ParsePbConfigFromFile("../conf/record.conf", &conf) == 0)
    << "parse task config file error!";

  // create work task
  for (int i = 0; i < conf.concurrent(); i++) {
    pid_t pid = fork();
    if (pid == 0) {
      Worker(conf);
      return 0;
    } else if (pid < 0) {
      LOG(ERROR) << "create record work task failed! error: "
        << strerror(errno);
    }
  }

  // create req proxy
  RecordProxy(conf);
  LOG(ERROR) << "WHY EXIT? ";

  return 0;
}
