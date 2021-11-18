#! /bin/sh

IMAGE_NAME="buildenv"
CONTAINER_NAME="env"

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
DOCKER_PATH=$FIRMWARE_ROOT/tools

printHelp ()
{
    echo "###################### Commands ######################"
    echo "### <NONE> -> Build docker image if does not exist"
    echo "### -f, --force -> Force rebuild of docker image"
    echo "### -h, --help -> Print help message"

    exit 1
}

deleteOldContainer ()
{
    if docker container inspect $CONTAINER_NAME &> /dev/null; then
        echo "Deleting docker container... "
        docker container stop $CONTAINER_NAME
        docker container rm $CONTAINER_NAME
    fi
}

deleteOldImage ()
{
    if docker image inspect $IMAGE_NAME &> /dev/null; then
        echo "Deleting docker image... "
        docker image rm $IMAGE_NAME
    fi
}

buildDockerImage ()
{
    docker build $DOCKER_PATH -t $IMAGE_NAME
}

conditionalBuildImage ()
{
    if ! docker image inspect $IMAGE_NAME &> /dev/null; then
        echo "Docker image does not exist.. building now"
        buildDockerImage
    fi

    deleteOldContainer
}

# Input Argument Parsing
for ((i=1; i<=$#; i++)) do
    arg=${!i}
    case "$arg" in
        -f|--force)
            FORCE_FLAG="TRUE"
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

if [[ $FORCE_FLAG == "TRUE" ]]; then
    deleteOldImage
    buildDockerImage
else
    conditionalBuildImage
fi
