#! /bin/bash

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
BUILD_DOCKER_SCRIPT=$FIRMWARE_ROOT/tools/build_docker_image.sh

TEMP_FIND_FILE=/workspace/formatter_files.txt

echoerr()
{
    echo "$@" 1>&2;
}

printHelp ()
{
    echo "###################### Commands ######################"
    echo "### <NONE> -> run formatter and replace code with formatted output"
    echo "### -c, --check -> Check if code is formatted correctly"
    echo "### -h, --help -> Print help message"

    exit 1
}

startDocker ()
{
    local IMAGE_NAME="buildenv"

    source "$BUILD_DOCKER_SCRIPT"

    docker create -it -v $FIRMWARE_ROOT:/workspace --name env $IMAGE_NAME bash > /dev/null
    docker start env > /dev/null
}

cleanupDocker ()
{
    docker rm -f env > /dev/null
}

runFormatter ()
{
    # get a list of all the files we want to run the formatter on
    docker exec -i env bash -c "find /workspace/. \( -name '*.c' -o -name '*.h' -o -path '/workspace/./dependencies' -prune \) -type f > $TEMP_FIND_FILE"

    if [[ $CHECK_FLAG == "TRUE" ]]; then
    # run formatter check
    FORMATTER_RUN_STATUS=$(docker exec -i env bash -c "uncrustify -c /workspace/tools/firmware_style.cfg -F $TEMP_FIND_FILE --check")
    else
    # run formatter inplace
    FORMATTER_RUN_STATUS=$(docker exec -i env bash -c "uncrustify -c /workspace/tools/firmware_style.cfg -F $TEMP_FIND_FILE --replace --no-backup")
    fi

    # delete file list file
    docker exec -i env bash -c "rm $TEMP_FIND_FILE"
}

# Input Argument Parsing
for ((i=1; i<=$#; i++)) do
    arg=${!i}
    case "$arg" in
        -c|--check)
            CHECK_FLAG="TRUE"
            ;;
        help|-h|--help|-help)
            printHelp
            ;;
        *)
            echoerr "Unknown input \"$arg\""
            printHelp
            exit 1
            ;;
    esac
done

startDocker
runFormatter
cleanupDocker

exit $FORMATTER_RUN_STATUS
