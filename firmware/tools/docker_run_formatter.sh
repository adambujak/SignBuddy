#! /bin/bash

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
BUILD_DOCKER_SCRIPT=$FIRMWARE_ROOT/tools/build_docker_image.sh

startDocker ()
{
    local IMAGE_NAME="buildenv"

    source "$BUILD_DOCKER_SCRIPT"

    docker create -it -v $GIT_ROOT:/workspace --name env $IMAGE_NAME bash > /dev/null
    docker start env > /dev/null
}

cleanupDocker ()
{
    docker rm -f env > /dev/null
}

runFormatter ()
{
    docker exec -i env bash -c "pushd /workspace/firmware/tools && bash run_formatter.sh $@"
    FORMATTER_RUN_STATUS=$?
}

startDocker
runFormatter
cleanupDocker

exit $FORMATTER_RUN_STATUS
