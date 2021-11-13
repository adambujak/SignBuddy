#! /bin/sh

BOARD_VERSION="${1:-DISCO}"

IMAGE_NAME="buildenv"

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
BUILD_DOCKER_SCRIPT=$FIRMWARE_ROOT/tools/build_docker_image.sh

source "$BUILD_DOCKER_SCRIPT"

docker create -it -v $FIRMWARE_ROOT:/workspace --name env $IMAGE_NAME bash > /dev/null
docker start env > /dev/null
docker exec -i env bash -c "cd /workspace/applications/SignBuddy && make BOARD_VERSION=$BOARD_VERSION"
docker rm -f env > /dev/null
