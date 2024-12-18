#!/usr/bin/env bash

PROJECT_NAME=$1
if [ -z "$PROJECT_NAME" ]; then
    echo "Usage: $0 <PROJECT_NAME> [platform_name]"
    exit 1
fi

PLATFORM_NAME=$2
if [ -z "$PLATFORM_NAME" ]; then
    PLATFORM_NAME="native"
    echo "No platform specified, using default platform: ${PLATFORM_NAME}"
fi


PROJECT_DIR=$3
if [ -z "$PROJECT_DIR" ]; then
    PROJECT_DIR="examples/${PROJECT_NAME}"
    echo "No project directory specified, using default project directory: ${PROJECT_DIR}"
fi

echo "Building example: ${PROJECT_NAME} for platform: ${PLATFORM_NAME}"
echo "-----------------------------------"
echo ""

BUILD_DIR="build/${PLATFORM_NAME}/${PROJECT_NAME}"
mkdir -p "${BUILD_DIR}"

# Configure using the Unix Makefiles generator
cmake -G "Unix Makefiles" -S . -B "${BUILD_DIR}" \
    -DPLATFORM_NAME="${PLATFORM_NAME}" \
    -DPROJECT="${PROJECT_NAME}" \
    -DPROJECT_CMAKE_DIR="${PROJECT_DIR}"

# Build only the requested example target from the specified build directory
cmake --build "${BUILD_DIR}" --target "${PROJECT_NAME}"

echo "Build complete. The executable should be in ${BUILD_DIR}/examples/${PROJECT_NAME}/"
