
# --- CMAKE SETUP ---

cmake_minimum_required(VERSION 3.13)

project(${PROJECT} C CXX)

# set the PROJECT_ROOT_DIR variable to the root directory of the project
set(DOODLE_PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_CMAKE_DIR})

# --- THIS ASSUMES THAT THE PROJECT IS CMAKE AS WELL ---

# now include the project cmakelists, found in the project directory
# this will be expected to provide a function called "build_project"
# this will be used to actually build the doodle project executable
include(${DOODLE_PROJECT_ROOT_DIR}/doodle.cmake)
message(STATUS "Building project ${PROJECT_NAME} (${PROJECT_CMAKE_DIR}) for platform ${PLATFORM_NAME}")

# create the executable target, for use in the project
add_executable(${PROJECT_NAME})

# --- COMPILING DOODLE CORE ---

message(STATUS "Building doodle core")

set(DOODLE_PARENT_DIR ${CMAKE_SOURCE_DIR})
# Collect all core .c and .h files recursively, through the core directory
file(GLOB_RECURSE DOODLE_CORE_SOURCES "${DOODLE_PARENT_DIR}/doodle/core/*.c")
file(GLOB_RECURSE DOODLE_CORE_HEADERS "${DOODLE_PARENT_DIR}/doodle/core/*.h")

# declare the doodle library as a static library
add_library(doodle STATIC
    ${DOODLE_CORE_SOURCES}
    ${DOODLE_CORE_HEADERS}
)

# also set the C standard to C99
set_property(TARGET doodle PROPERTY C_STANDARD 99)

# add the core sources to the doodle library
target_sources(doodle PRIVATE ${DOODLE_CORE_SOURCES} ${DOODLE_CORE_HEADERS})

# --- COMPILING PLATFORM : This assumes that the platform is CMake as well ---

message(STATUS "Building platform ${PLATFORM_NAME}")

set(PLATFORM_ROOT_DIR ${DOODLE_PARENT_DIR}/doodle/platforms)

# try to find the platform directory
set(PLATFORM_DIR ${PLATFORM_ROOT_DIR}/${PLATFORM_NAME})
if(NOT EXISTS ${PLATFORM_DIR})
    message(FATAL_ERROR "Specified platform '${PLATFORM_NAME}' not found at ${PLATFORM_DIR}")
endif()

# set the includes for doodle
target_include_directories(doodle PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PLATFORM_DIR}
)

# declare the platform as a library
add_library(${PLATFORM_NAME} STATIC)

if(EXISTS ${PLATFORM_CMAKE_FILE})
    # The platform.cmake file should define at least:
    #   PLATFORM_SOURCES (list of .c files)
    #   PLATFORM_HEADERS (list of .h files, optional)
    # and optionally manipulate include directories or link libraries.
    message(STATUS "Using platform-specific configuration from ${PLATFORM_CMAKE_FILE}")
    include(${PLATFORM_CMAKE_FILE})
else()
    # No custom platform cmake, just glob all .c/.h files in that directory
    message(STATUS "No platform-specific configuration found, using default sources from ${PLATFORM_DIR}")
    file(GLOB PLATFORM_SOURCES "${PLATFORM_DIR}/*.c")
    file(GLOB PLATFORM_HEADERS "${PLATFORM_DIR}/*.h")

    # include the platform directory for headers
    target_include_directories(${PLATFORM_NAME} PUBLIC ${PLATFORM_DIR})

    # add the platform sources to the platform library
    target_sources(${PLATFORM_NAME} PRIVATE ${PLATFORM_SOURCES})

endif()

message("Finished compiling platform ${PLATFORM_NAME}")


# --- ADDING SOURCES TO THE DOODLE LIBRARY ---

message(STATUS "Adding core sources to doodle library")


message(STATUS "Linking platform ${PLATFORM_NAME} to doodle library")

# link the doodle library to the platform library
target_link_libraries(doodle PUBLIC ${PLATFORM_NAME})

# --- LINKING PLATFORM-SPECIFIC MAIN FILE ---
# Because the platform main file is not part of the doodle library, or the project,
# we need to add it to the project separately.

# update PLATFORM_MAIN_FILE to be in the platform directory
set(PLATFORM_MAIN_FILE ${PLATFORM_DIR}/${PLATFORM_MAIN_FILE})

if(DEFINED PLATFORM_MAIN_FILE)
    set(PLATFORM_MAIN_FILE ${PLATFORM_MAIN_FILE})
else()
    set(PLATFORM_MAIN_FILE ${PLATFORM_DIR}/main.c)
    message(STATUS "No platform main file path provided, using default main.c from ${PLATFORM_MAIN_FILE}")
endif()

if(EXISTS ${PLATFORM_MAIN_FILE})
    message(STATUS "Using platform-specific main.c from ${PLATFORM_MAIN_FILE}")
    target_sources(doodle PRIVATE ${PLATFORM_MAIN_FILE})
else()
    message(STATUS "No platform-specific main.c found, using default main.c from ${CMAKE_CURRENT_SOURCE_DIR}")
    target_sources(doodle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
endif()

# now add the platform main file to the sources for the PROJECT_NAME executable
target_sources(${PROJECT_NAME} PRIVATE ${PLATFORM_MAIN_FILE})


# If the platform.cmake sets additional properties (like target_link_libraries calls),
# they will already be in effect after being included.
message(STATUS "Project root directory: ${DOODLE_PROJECT_ROOT_DIR}")

# call the build_project function to add the project sources to the executable
build_project()

# now all sources have been added to the project, we can build it
# this will be the final executable
message(STATUS "Building executable ${PROJECT_NAME}")
# now finally link the doodle library to the project
target_link_libraries(${PROJECT_NAME} PRIVATE doodle)