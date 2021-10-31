#! /bin/sh

IMAGE_NAME="buildenv"

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
DOCKER_PATH=$FIRMWARE_ROOT/tools

if ! docker image inspect $IMAGE_NAME &> /dev/null; then
    echo "Docker image does not exist.. building now"
    docker build $DOCKER_PATH -t $IMAGE_NAME
fi


