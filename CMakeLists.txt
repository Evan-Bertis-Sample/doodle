cmake_minimum_required(VERSION 3.13)
project(${PROJECT} C)


# Set default platform name if not provided
set(PLATFORM_NAME "native" CACHE STRING "Platform name")

# set the PROJECT_ROOT_DIR variable to the root directory of the project
set(DOODLE_PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_CMAKE_DIR})

# now include the project cmakelists, found in the project directory
# this will be expected to provide a function called "build_project"
# this will be used to actually build the doodle project executable
include(${DOODLE_PROJECT_ROOT_DIR}/doodle.cmake)

message(STATUS "Building project ${PROJECT_NAME} (${PROJECT_CMAKE_DIR}) for platform ${PLATFORM_NAME}")

# Add the doodle library
add_subdirectory(doodle)


message(STATUS "Project root directory: ${DOODLE_PROJECT_ROOT_DIR}")

# call the build_project function
build_project()


