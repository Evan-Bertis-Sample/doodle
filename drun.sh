#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

# Function to display usage information
usage() {
    echo "Usage: $0 <example_name> [platform_name] [-- <args>...]"
    echo
    echo "  example_name    The name of the example to build and run (e.g., blank)"
    echo "  platform_name   (Optional) The platform to build for (default: native)"
    echo "  -- <args>       (Optional) Arguments to pass to the executable"
    echo
    echo "Example:"
    echo "  $0 blank"
    echo "  $0 blank other_platform -- arg1 arg2"
    exit 1
}

# Check if at least one argument is provided
if [ -z "$1" ]; then
    usage
fi

# Parse arguments
EXAMPLE_NAME="$1"
shift

# Default platform name
PLATFORM_NAME="native"

# Initialize array for additional executable arguments
EXEC_ARGS=()

# Check if the next argument is a platform name or the '--' separator
if [ ! -z "$1" ] && [ "$1" != "--" ]; then
    PLATFORM_NAME="$1"
    shift
fi

# If there are remaining arguments, and the first one is '--', treat the rest as executable arguments
if [ "$1" == "--" ]; then
    shift
    EXEC_ARGS=("$@")
fi

# Define the build directory structure
BUILD_DIR="build/${PLATFORM_NAME}/${EXAMPLE_NAME}"

# Check if build directory exists; if not, create it
if [ ! -d "${BUILD_DIR}" ]; then
    mkdir -p "${BUILD_DIR}"
fi

# Run the build script
echo "Building example '${EXAMPLE_NAME}' for platform '${PLATFORM_NAME}'..."
./dbuild.sh "${EXAMPLE_NAME}" "${PLATFORM_NAME}"

# Define the path to the built executable
EXECUTABLE="${BUILD_DIR}/${EXAMPLE_NAME}"

# Verify that the executable exists and is executable
if [ ! -f "${EXECUTABLE}" ]; then
    echo "Error: Executable '${EXECUTABLE}' not found."
    echo "Ensure that the build was successful."
    exit 1
fi

if [ ! -x "${EXECUTABLE}" ]; then
    echo "Setting execute permission for '${EXECUTABLE}'."
    chmod +x "${EXECUTABLE}"
fi

# Run the executable with any additional arguments
echo "Running executable '${EXECUTABLE}'..."
"${EXECUTABLE}" "${EXEC_ARGS[@]}"
