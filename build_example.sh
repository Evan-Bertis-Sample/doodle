#!/usr/bin/env bash

EXAMPLE_NAME=$1
if [ -z "$EXAMPLE_NAME" ]; then
    echo "Usage: $0 <example_name> [platform_name]"
    exit 1
fi

PLATFORM_NAME=$2
if [ -z "$PLATFORM_NAME" ]; then
    PLATFORM_NAME="native"
fi

# Define the build directory structure
BUILD_DIR="build/${PLATFORM_NAME}/${EXAMPLE_NAME}"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Run CMake configuration with the specified platform and example
cmake -DPLATFORM_NAME="${PLATFORM_NAME}" -DEXAMPLE="${EXAMPLE_NAME}" ../.. 

# Build only the requested example target
cmake --build . --target "${EXAMPLE_NAME}"

echo "Build complete. The executable should be in ${BUILD_DIR}/examples/${EXAMPLE_NAME}/"
