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

BUILD_DIR="build/${PLATFORM_NAME}/${EXAMPLE_NAME}"
mkdir -p "${BUILD_DIR}"

# Use -S and -B to clearly specify source and build directories
cmake -B "${BUILD_DIR}" -DPLATFORM_NAME="${PLATFORM_NAME}" -DEXAMPLE="${EXAMPLE_NAME}"

# Now build only the requested example target from the specified build directory
cmake --build "${BUILD_DIR}" --target "${EXAMPLE_NAME}"

echo "Build complete. The executable should be in ${BUILD_DIR}/examples/${EXAMPLE_NAME}/"
