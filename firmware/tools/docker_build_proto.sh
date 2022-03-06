#! /bin/bash

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
BUILD_DOCKER_SCRIPT=$FIRMWARE_ROOT/tools/build_docker_image.sh

startDocker ()
{
    local IMAGE_NAME="buildenv"

    bash "$BUILD_DOCKER_SCRIPT"

    docker create -it -v $GIT_ROOT:/workspace --name env $IMAGE_NAME bash > /dev/null
    docker start env > /dev/null
}

cleanupDocker ()
{
    docker rm -f env > /dev/null
}

buildProto ()
{
    docker exec -i env bash -c "pushd /workspace/protobuf > /dev/null && bash build_proto.sh"
    TEST_RUN_STATUS=$?
}

startDocker
buildProto
cleanupDocker

exit $TEST_RUN_STATUS
