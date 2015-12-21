
#ifndef SERVICE_RECORD_CLIENT_RECORDCLIENT_H_
#define SERVICE_RECORD_CLIENT_RECORDCLIENT_H_

#include <iostream>
#include <memory>
#include <string>
#include "thirdparty/grpc++/grpc++.h"
#include "proto/record.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class RecordClient {
  public:
    RecordClient()
      : stub_(RecordServer::NewStub(
      grpc::CreateChannel("10.237.138.209:9527", grpc::InsecureCredentials()))) {}

    bool Send(const RecordReq &req, RecordRes *res) {
      ClientContext context;
      Status status = stub_->HandleRecord(&context, req, res);
      return status.ok();
    }

    std::string Error() {
      return err_;
    }

  private:
    std::unique_ptr<RecordServer::Stub> stub_;
    std::string err_;
};

#endif // SERVICE_RECORD_CLIENT_RECORDCLIENT_H_
