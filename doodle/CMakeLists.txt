# set the minimum required CMake version
cmake_minimum_required(VERSION 3.13)

# Collect all core .c and .h files recursively, through the core directory
file(GLOB_RECURSE DOODLE_CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/core/*.c")
file(GLOB_RECURSE DOODLE_CORE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/core/*.h")


set(PLATFORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/${PLATFORM_NAME})
if(NOT EXISTS ${PLATFORM_DIR})
    message(FATAL_ERROR "Specified platform '${PLATFORM_NAME}' not found at ${PLATFORM_DIR}")
endif()

# Check if there's a platform-specific CMake configuration file
set(PLATFORM_CMAKE_FILE ${PLATFORM_DIR}/platform.cmake)
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
endif()

add_library(doodle STATIC
    ${DOODLE_CORE_SOURCES}
    ${DOODLE_CORE_HEADERS}
    ${PLATFORM_SOURCES}
    ${PLATFORM_HEADERS}
)

# also set the C standard to C99
set_property(TARGET doodle PROPERTY C_STANDARD 99)

target_include_directories(doodle PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PLATFORM_DIR}
)


# In the platform directory, we will be expecting a main.c file
# This will be the entry point for the application! 
# Why do we do this? Because different platforms may have different entry points.
# Most specifically, arduino sketches have a different entry point than a normal C program.

# Check if there's a platform-specific main.c file
# we may have been passed in a platform main file path, lets check if it exists
# if not, we will use the default main.c file

# update PLATFORM_MAIN_FILE to be in the platform directory
set(PLATFORM_MAIN_FILE ${PLATFORM_DIR}/${PLATFORM_MAIN_FILE})

if (DEFINED PLATFORM_MAIN_FILE)
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
