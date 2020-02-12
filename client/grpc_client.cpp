#include <iostream>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "../arithmetic_service.pb.h"
#include "../arithmetic_service.grpc.pb.h"

const std::string address = "0.0.0.0:19988";


int main(int argc, char* argv[]) {

    std::cout << "[INFO] Arithmetic Client." << std::endl;

    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
    std::unique_ptr<Arithmetic::Interface::Stub> stub = Arithmetic::Interface::NewStub(channel);

    Arithmetic::ReqOperand request;
    request.set_num_1(20);
    request.set_num_2(30);

    Arithmetic::RspOperand response;

    grpc::Status status;

    {
        grpc::ClientContext context;

        status = stub->GetSum(&context, request, &response);
        if(!status.ok()) {
            std::cout << "[ERROR] call GetSum failed." << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "[INFO] GetSum result: " << response.num() << std::endl;
    }

    {
        grpc::ClientContext context;

        status = stub->GetSub(&context, request, &response);
        if(!status.ok()) {
            std::cout << "[ERROR] call GetSub failed." << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "[INFO] GetSub result: " << response.num() << std::endl;
    }



    return EXIT_SUCCESS;
}