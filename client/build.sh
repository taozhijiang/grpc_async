
g++ -std=c++11 grpc_client.cpp -o ../build/bin/grpc_client \
-I../../../opensource/xtra_rhel7.x/google_prefix/include \
../arithmetic_service.grpc.pb.cc ../arithmetic_service.pb.cc \
../../../opensource/xtra_rhel7.x/google_prefix/lib/libprotoc.a \
../../../opensource/xtra_rhel7.x/google_prefix/lib/libprotobuf.a \
../../../opensource/xtra_rhel7.x/google_prefix/lib/libgrpc++.a \
../../../opensource/xtra_rhel7.x/google_prefix/lib/libgrpc.a \
../../../opensource/xtra_rhel7.x/google_prefix/lib/libgpr.a \
-lpthread -lz
