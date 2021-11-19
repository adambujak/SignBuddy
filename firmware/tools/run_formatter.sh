#! /bin/bash

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
BUILD_DOCKER_SCRIPT=$FIRMWARE_ROOT/tools/build_docker_image.sh

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

getBlackListFindExclusion()
{
    local blackListPath=${1}
    local searchDir=${2}
    # return expected:
    # \( -path [path1] -o -path [path2] \)
    local findExclusionString="\( "

    local isFirstLine="1"
    # Read rest of file and add as exclusion paths
    while IFS= read -r line; do
        path="${line}"

        # If first line in file, don't prepend -o
        if [ "1" == "${isFirstLine}" ]; then
            findExclusionString="${findExclusionString} -path $searchDir/${line}"
            isFirstLine="0"
        else
            findExclusionString="${findExclusionString} -o -path $searchDir/${line}"
        fi
    done < ${blackListPath}
    echo "${findExclusionString} \)"
}

findFiles ()
{
    local OUTPUT_FILE=${1}
    local SEARCH_DIR="$FIRMWARE_ROOT"
    local EXCLUDE_FILE_PATH="$FIRMWARE_ROOT/tools/formatter_blacklist.txt"

    local blackListFindExclusion=$(getBlackListFindExclusion ${EXCLUDE_FILE_PATH} ${SEARCH_DIR})
    local fileInclusionString=" \( -name '*.c' -o -name '*.h' \)"
    local findParams="$SEARCH_DIR -type d $blackListFindExclusion -prune -o $fileInclusionString -print"
    local findCommand="find $findParams"
    eval "$findCommand" > $OUTPUT_FILE
}

runFormatter ()
{
    # get a list of all the files we want to run the formatter on
    local TEMP_FILE="formatter_file_list.temp"
    findFiles $TEMP_FILE

    if [[ $CHECK_FLAG == "TRUE" ]]; then
    # run formatter check
    uncrustify -c $FIRMWARE_ROOT/tools/firmware_style.cfg -F $TEMP_FILE --check
    FORMATTER_RUN_STATUS=$?
    else
    # run formatter inplace
    uncrustify -c $FIRMWARE_ROOT/tools/firmware_style.cfg -F $TEMP_FILE --replace --no-backup
    FORMATTER_RUN_STATUS=$?
    fi

    # delete file list file
    rm $TEMP_FILE
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

runFormatter

exit $FORMATTER_RUN_STATUS
