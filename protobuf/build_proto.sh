#! /bin/bash

IMAGE_NAME="buildenv"

GIT_ROOT=$(git rev-parse --show-toplevel)
FIRMWARE_ROOT=$GIT_ROOT/firmware
SOFTWARE_ROOT=$GIT_ROOT/software
NANOPB_ROOT=$FIRMWARE_ROOT/dependencies/nanopb/generator
PROTO_PATH=$GIT_ROOT/protobuf

C_GENERATE_PATH=$FIRMWARE_ROOT/applications/SignBuddy
PYTHON_GENERATE_PATH=$SOFTWARE_ROOT/sidekick
JAVA_GENERATE_PATH=$SOFTWARE_ROOT/android_app/app/src/main/java

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

pushd $JAVA_GENERATE_PATH > /dev/null
cp $PROTO_PATH/sign_buddy.proto .
protoc -I=. --javalite_out=. sign_buddy.proto
rm sign_buddy.proto
popd
