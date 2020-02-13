#ifndef __SERVICE_IMPL__
#define __SERVICE_IMPL__

#include "arithmetic_service.pb.h"
#include "arithmetic_service.grpc.pb.h"


class ServiceImpl final : public Arithmetic::Interface::Service {

public:
    grpc::Status GetSum(grpc::ServerContext* context,
            const Arithmetic::ReqOperand* req,
            Arithmetic::RspOperand* rsp) override {

        rsp->set_num(req->num_1() + req->num_2());
        return grpc::Status::OK;
    }

    grpc::Status GetSub(grpc::ServerContext* context,
                        const Arithmetic::ReqOperand* req,
                        Arithmetic::RspOperand* rsp) override {

        rsp->set_num(req->num_1() - req->num_2());
        return grpc::Status::OK;
    }

};


#endif // __SERVICE_IMPL__