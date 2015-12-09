
#include <iostream>
#include <memory>
#include "common/utils/PbUtils.h"
#include "common/utils/StringUtils.h"
#include "common/utils/GLogWrapper.h"
#include "service/interact/proto/InteractConf.pb.h"
#include "proto/IPC/InteractIpc.grpc.pb.h"
#include "grpc++/grpc++.h"
#include "common/msgque/SupperQueue.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using namespace std;

class InteractProxy final : public Interact::Service  {
  public:
  InteractProxy(const InteractConf &conf) : conf_(conf) {};

  bool Init() {
    CHECK(FileUtils::RecursivelyCreateDir(conf_.dir().invoice_dir(),
          S_IRWXO | S_IRWXU | S_IRWXG)) << "create interact invoice dir failed!";
    CHECK(FileUtils::RecursivelyCreateDir(conf_.dir().cache_dir(),
          S_IRWXO | S_IRWXU | S_IRWXG)) << "create interact cache dir failed!";
    CHECK(FileUtils::RecursivelyCreateDir(conf_.dir().notice_dir(),
          S_IRWXO | S_IRWXU | S_IRWXG)) << "create interact notice dir failed!";
    CHECK(FileUtils::RecursivelyCreateDir(conf_.dir().statistic_dir(),
          S_IRWXO | S_IRWXU | S_IRWXG)) << "create interact statistic dir failed!";

    queue_ = new imque::SupperQueue<InteractReq>(conf_.queue().shm_size(),
                conf_.queue().queue_path());
     return *queue_ ? true : false;
  }

  Status HandleInteract(ServerContext* context, const InteractReq* req,
      InteractRes *res) {
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
    InteractConf conf_;
    imque::SupperQueue<InteractReq> *queue_;
};

int main(int argc, char** argv) {
  if(!InitGlog(argv[0], "../log")) {
    std::cout << "task init failed! name: " << argv[0] << std::endl;
    return -1;
  }

  InteractConf conf;
  CHECK(ParsePbConfigFromFile("../conf/interact.conf", &conf) == 0)
    << "parse task config file error!";


  string server_address(string("0.0.0.0:") + value_to_string(conf.port()));
  InteractProxy service(conf);

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();

  return 0;
}
