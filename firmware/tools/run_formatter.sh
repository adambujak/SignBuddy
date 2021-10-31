#! /bin/sh

IMAGE_NAME="buildenv"

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
BUILD_DOCKER_SCRIPT=$FIRMWARE_ROOT/tools/build_docker_image.sh

TEMP_FIND_FILE=/workspace/formatter_files.txt

source "$BUILD_DOCKER_SCRIPT"

docker create -it -v $FIRMWARE_ROOT:/workspace --name env $IMAGE_NAME bash > /dev/null
docker start env > /dev/null
docker exec -i env bash -c "find /workspace/. \( -name '*.c' -o -name '*.h' -o -path '/workspace/./dependencies' -prune \) -type f > $TEMP_FIND_FILE"
docker exec -i env bash -c "uncrustify -c /workspace/tools/firmware_style.cfg -F $TEMP_FIND_FILE --replace --no-backup"
docker exec -i env bash -c "rm $TEMP_FIND_FILE"
docker rm -f env > /dev/null
