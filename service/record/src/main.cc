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
    Status HandleRecord(ServerContext* context, const RecordReq* req,
        RecordRes *res) {
      LOG(ERROR) << "got a request:\n-------------------\n" << req->Utf8DebugString();
      pid_t pid = fork();
      if (pid == 0) {
        RecordWorker worker;
        worker.Handle(*req);
        exit(0);
      } else if (pid < 0) {
        LOG(ERROR) << "create record work task failed! error: "
          << strerror(errno);
        res->set_code(-1);
      } else {
        res->set_code(0);
      }
      res->set_id(req->id());
      return Status::OK;
    }
};

void RecordProxy(const RecordConf &conf) {
  string server_address(conf.host() + ":" + value_to_string(conf.port()));
  RecordService service;
  LOG(ERROR) << "init record service failed!";
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();
  LOG(ERROR) << "why enter here!";

  return;
}

int main(int argc, char** argv) {
  if(!InitGlog(argv[0], "../log")) {
    std::cout << "task init failed! name: " << argv[0] << std::endl;
    return -1;
  }

  RecordConf conf;
  CHECK(ParsePbConfigFromFile("../conf/record.conf", &conf) == 0)
    << "parse task config file error!";

  // create req proxy
  RecordProxy(conf);
  LOG(ERROR) << "WHY EXIT? ";

  return 0;
}
