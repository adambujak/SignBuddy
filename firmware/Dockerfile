FROM ubuntu:20.04

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y make gcc-arm-none-eabi

VOLUME /root/env
WORKDIR /root/env

# Build docker image with:
#     docker build . -t buildenv
# Start up docker container with:
#     docker run --rm -it -v ${pwd}:/root/env buildenv
