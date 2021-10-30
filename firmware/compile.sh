#! /bin/sh

docker create -it -v $(pwd):/workspace/firmware --name env buildenv bash
docker start env
docker exec -i env bash -c "cd /workspace/firmware/applications/SignBuddy && make"
docker rm -f env
