#! /bin/sh

IMAGE_NAME="buildenv"
CONTAINER_NAME="env"

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
DOCKER_PATH=$FIRMWARE_ROOT/tools

if ! docker image inspect $IMAGE_NAME &> /dev/null; then
    echo "Docker image does not exist.. building now"
    docker build $DOCKER_PATH -t $IMAGE_NAME
fi

if ! docker container inspect $CONTAINER_NAME &> /dev/null; then
    echo "Deleting docker container... "
    docker container stop $CONTAINER_NAME
    docker container rm $CONTAINER_NAME
fi
