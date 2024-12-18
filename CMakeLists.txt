cmake_minimum_required(VERSION 3.13)
project(${PROJECT} C)

# Set default platform name if not provided
set(PLATFORM_NAME "native" CACHE STRING "Platform name")

# Optionally, you can list allowed platforms here
# set(AVAILABLE_PLATFORMS native other_platform ...)


message(STATUS "Building project ${PROJECT_NAME} (${PROJECT_CMAKE_DIR}) for platform ${PLATFORM_NAME}")

# Add the doodle library
add_subdirectory(doodle)

# now add the CMAKELists.txt for the example project, passed in as an argument
add_subdirectory(${PROJECT_CMAKE_DIR})

