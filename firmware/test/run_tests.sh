#! /bin/bash

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
TEST_ROOT=$FIRMWARE_ROOT/test
BUILD_DOCKER_SCRIPT=$FIRMWARE_ROOT/tools/build_docker_image.sh

findMakefiles ()
{
    local SEARCH_DIR="$TEST_ROOT"

    local fileInclusionString=" \( -name 'Makefile' \)"
    local findParams="$SEARCH_DIR -type d \( -path $SEARCH_DIR/Unity \) -prune -o $fileInclusionString -print | sed 's|/[^/]*$||'"
    local findCommand="find $findParams"
    echo $(eval "$findCommand")
}

runTests ()
{
    # get a list of all the files we want to run the formatter on
    local makefilePaths=$(findMakefiles)
    local CYAN='\033[0;36m'
    local NC='\033[0m'

    for path in ${makefilePaths/"\n"/ };
    do
        echo -e $CYAN
        echo "==================================================================="
        echo "Running test in $path"
        echo "==================================================================="
        echo -e $NC
        make -C $path
    done
}

runTests

