#ifndef __SERVICE_IMPL_ASYNC__
#define __SERVICE_IMPL_ASYNC__

#include <thread>

#include "arithmetic_service.pb.h"
#include "arithmetic_service.grpc.pb.h"


class CallData final {

public:
    CallData(Arithmetic::Interface::AsyncService* service, grpc::ServerCompletionQueue* completion_queue):
        cd_service_(service),
        cd_completion_queue_(completion_queue),
        ctx_(),
        rsp_operand_responder_(&ctx_),
        status_(CallStatus::CREATE) {

        // do business
        Proceed();
    }

    ~CallData() = default;

    void Proceed() {

        if(status_ == CallStatus::CREATE) {

            status_ = CallStatus::PROCESS;
            cd_service_->RequestGetSum(&ctx_,
                    &req_operand_, &rsp_operand_responder_,
                    cd_completion_queue_, cd_completion_queue_, this);

            std::cout << "[INFO] CREATE -> PROCESS, tag: " << reinterpret_cast<uint64_t>(this) << std::endl;
        }
        else if (status_ == CallStatus::PROCESS) {

            rsp_operand_.set_num(req_operand_.num_1() + req_operand_.num_2());

            status_ = CallStatus::FINISH;
            rsp_operand_responder_.Finish(rsp_operand_, grpc::Status::OK, this);

            std::cout << "[INFO] PROCESS -> FINISH, tag: " << reinterpret_cast<uint64_t>(this) << std::endl;
        }
        else {

            assert(status_ == CallStatus::FINISH);
            if(status_ != CallStatus::FINISH) {
                std::cout << "[ERROR] status validation failed: " << static_cast<int>(status_) << std::endl;
                // ???
            }

            std::cout << "[INFO] FINISH -> DONE, tag: " << reinterpret_cast<uint64_t>(this) << std::endl;


            // 准备下一个请求接收
            new CallData(cd_service_, cd_completion_queue_);

            delete this;
        }
    }


private:
    Arithmetic::Interface::AsyncService* cd_service_;
    grpc::ServerCompletionQueue* cd_completion_queue_;

    grpc::ServerContext ctx_;

    Arithmetic::ReqOperand req_operand_;

    Arithmetic::RspOperand rsp_operand_;
    grpc::ServerAsyncResponseWriter<Arithmetic::RspOperand> rsp_operand_responder_;


    enum class CallStatus : uint8_t {
        CREATE,
        PROCESS,
        FINISH,
    };
    enum CallStatus status_;
};



class ServiceImplAsync final {

public:

    ServiceImplAsync() = default;
    ~ServiceImplAsync() {
        completion_queue_->Shutdown();
    }

    bool init(grpc::ServerBuilder& builder) {
        completion_queue_ = builder.AddCompletionQueue();
        builder.RegisterService(&service_);
        return true;
    }

    void RunBiz(size_t thread_sz) {
        for(size_t i=0; i<thread_sz; ++i) {
            std::cout << "[INFO] create thread index " << i << " ..." << std::endl;
            threads_.emplace_back(std::thread(std::bind(&ServiceImplAsync::ProcessBiz, this)));
        }
    }

    void ProcessBiz() {

        new CallData(&service_, completion_queue_.get());

        void* tag;
        bool ok;

        while(true) {

            bool code = completion_queue_->Next(&tag, &ok);
            if(!code || !ok) {
                std::cout << "[ERROR] completion_queue Next error." << std::endl;
                break;
            }

            std::cout << "[INFO] post request for proceed ..." << std::endl;
            static_cast<CallData *>(tag)->Proceed();
        }

        std::cout << "[INFO] thread terminated..." << std::endl;
    }

private:
    Arithmetic::Interface::AsyncService service_;
    std::unique_ptr<grpc::ServerCompletionQueue> completion_queue_;

    std::vector<std::thread> threads_;
};


#endif // __SERVICE_IMPL_ASYNC__