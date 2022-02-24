# Build docker image with:
#     docker build . -t buildenv
FROM ubuntu:20.04

RUN mkdir /workspace

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get update
RUN apt-get install -y make gcc-arm-none-eabi uncrustify git ruby build-essential python3-pip
RUN pip install protobuf grpcio-tools
RUN apt-get install -y autoconf automake libtool curl g++ unzip
RUN curl -Ls https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.0.1-javalite.zip --output protobuf.zip
RUN unzip -q protobuf.zip
WORKDIR "/protobuf-3.0.1-javalite"
RUN autoreconf -i
RUN ./configure
RUN make
RUN make install
RUN ldconfig
