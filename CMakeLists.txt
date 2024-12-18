cmake_minimum_required(VERSION 3.10)
project(doodle_project C)

# Make sure we can find includes from doodle/include
include_directories(${CMAKE_SOURCE_DIR}/doodle/include)

# Optionally allow specifying the platform
# You can invoke this as: cmake -DPLATFORM_NAME=native ..
option(PLATFORM_NAME "Platform name" "native")

# Add the doodle library subdirectory
add_subdirectory(doodle)
