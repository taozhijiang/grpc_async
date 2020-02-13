#include <iostream>
#include <cstdlib>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>

const std::string address = "0.0.0.0:19988";

#include "service_impl.h"
#include "service_impl_async.h"

static int RunServer() {

    ServiceImpl service {};

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "[INFO] server started at " << address << std::endl;

    // loop forever
    server->Wait();

    return 0;
}

static int RunServerAsync() {

    ServiceImplAsync service {};

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.SetMaxSendMessageSize(std::numeric_limits<int>::max());
    builder.SetMaxReceiveMessageSize(std::numeric_limits<int>::max());

    if(!service.init(builder)) {
        std::cout << "[ERROR] ServiceImplAsync init failed." << std::endl;
        return -1;
    }

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "[INFO] server started at " << address << std::endl;

    service.RunBiz(5);
    server->Wait();

    return 0;
}


int main(int argc, char* argv[]) {

    std::cout << "[INFO] begin grpc_async demo test." << std::endl;


    // Sync
    // RunServer();

    // Async
    RunServerAsync();

    return EXIT_SUCCESS;
}
