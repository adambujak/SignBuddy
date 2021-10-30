#! /bin/sh

docker create -it --name env buildenv bash
docker start env
docker exec -i env bash -c "cd /workspace/firmware && rm -rf applications libraries"
docker cp -a applications env:/workspace/firmware/
docker cp -a libraries env:/workspace/firmware/
docker exec -i env bash -c "cd /workspace/firmware/applications/SignBuddy && make"
docker cp -a env:/workspace/firmware/applications/SignBuddy/_build applications/SignBuddy/
docker rm -f env
