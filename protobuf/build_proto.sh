#! /bin/bash

IMAGE_NAME="buildenv"

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
NANOPB_ROOT=$FIRMWARE_ROOT/dependencies/nanopb/generator
PROTO_PATH=$GIT_ROOT/protobuf

C_GENERATE_PATH=$FIRMWARE_ROOT/applications/SignBuddy
PYTHON_GENERATE_PATH=$GIT_ROOT/software

pushd $C_GENERATE_PATH > /dev/null
cp $PROTO_PATH/sign_buddy.proto .
python3 $NANOPB_ROOT/nanopb_generator.py sign_buddy.proto
rm sign_buddy.proto
popd

pushd $PYTHON_GENERATE_PATH > /dev/null
cp $PROTO_PATH/sign_buddy.proto .
protoc -I=. --python_out=. sign_buddy.proto
rm sign_buddy.proto
popd

pushd $FIRMWARE_ROOT > /dev/null
bash ./tools/run_formatter.sh
popd

