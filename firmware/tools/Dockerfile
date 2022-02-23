# Build docker image with:
#     docker build . -t buildenv
FROM ubuntu:20.04

RUN mkdir /workspace

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get update
RUN apt-get install -y make gcc-arm-none-eabi uncrustify git ruby build-essential python3-pip
RUN apt-get install -y protobuf-compiler
RUN pip install protobuf grpcio-tools
