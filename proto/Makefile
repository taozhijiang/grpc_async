IDLS=$(wildcard *.proto)
OBJS=$(IDLS:%.proto=../%.pb.cc)
PREFIX=/home/user/repos/opensource/xtra_rhel7.x/google_prefix/bin

all: ${OBJS}
	@echo "begin to generate proto cpp source codes."

../%.pb.cc: %.proto
	${PREFIX}/protoc -I./ --cpp_out=../ $^
	${PREFIX}/protoc -I./ --grpc_out=../ --plugin=protoc-gen-grpc=${PREFIX}/grpc_cpp_plugin $^