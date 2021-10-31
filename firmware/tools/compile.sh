#! /bin/sh

IMAGE_NAME="buildenv"
GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=${GIT_ROOT}/firmware

if ! docker image inspect ${IMAGE_NAME} &> /dev/null; then
    echo "Docker image does not exist.. building now"
    docker build . -t buildenv
fi

docker create -it -v ${FIRMWARE_ROOT}:/workspace --name env ${IMAGE_NAME} bash > /dev/null
docker start env > /dev/null
docker exec -i env bash -c "cd /workspace/applications/SignBuddy && make"
docker rm -f env > /dev/null