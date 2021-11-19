#! /bin/bash

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
BUILD_DOCKER_SCRIPT=$FIRMWARE_ROOT/tools/build_docker_image.sh
TEST_PARAMS=$@

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

runTests ()
{
    docker exec -i env bash -c "pushd /workspace/firmware/test > /dev/null && bash run_tests.sh $TEST_PARAMS"
    TEST_RUN_STATUS=$?
}

startDocker
runTests
cleanupDocker

exit $TEST_RUN_STATUS
