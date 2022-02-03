#! /bin/bash

BOARD_VERSION="${1:-NUCLEO}"

IMAGE_NAME="buildenv"

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
NANOPB_ROOT=/workspace/dependencies/nanopb/generator
BUILD_DOCKER_SCRIPT=$FIRMWARE_ROOT/tools/build_docker_image.sh

bash $BUILD_DOCKER_SCRIPT

docker create -it -v $FIRMWARE_ROOT:/workspace --name env $IMAGE_NAME bash > /dev/null
docker start env > /dev/null
docker exec -i env bash -c "cd /workspace/applications/SignBuddy && python3 $NANOPB_ROOT/protoc --plugin=protoc-gen-nanopb=$NANOPB_ROOT/protoc-gen-nanopb --nanopb_out=. SignBuddy.proto --proto_path=."
BUILD_STATUS=$?
docker rm -f env > /dev/null

exit $BUILD_STATUS
