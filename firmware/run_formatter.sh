#! /bin/sh

IMAGE_NAME="buildenv"

if ! docker image inspect ${IMAGE_NAME} &> /dev/null; then
    echo "Docker image does not exist.. building now"
    docker build . -t buildenv
fi

docker create -it -v /$(pwd):/workspace --name env ${IMAGE_NAME} bash > /dev/null
docker start env > /dev/null
docker exec -i env bash -c "find /workspace/. \( -name '*.c' -o -name '*.h' -o -path '/workspace/./dependencies' -prune \) -type f > /workspace/formatter_files.txt"
docker exec -i env bash -c "uncrustify -c /workspace/firmware_style.cfg -F /workspace/formatter_files.txt --replace --no-backup"
docker exec -i env bash -c "rm /workspace/formatter_files.txt"
docker rm -f env > /dev/null
