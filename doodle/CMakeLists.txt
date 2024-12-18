# set the minimum required CMake version
cmake_minimum_required(VERSION 3.13)


# Collect all core .c and .h files in doodle/ and doodle/include/
file(GLOB DOODLE_CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB DOODLE_CORE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB DOODLE_INCLUDE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB DOODLE_INCLUDE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.c")

# print out all of the core sources
message(STATUS "Core sources: ${DOODLE_CORE_SOURCES}")
message(STATUS "Core headers: ${DOODLE_CORE_HEADERS}")
message(STATUS "Include headers: ${DOODLE_INCLUDE_HEADERS}")
message(STATUS "Include sources: ${DOODLE_INCLUDE_SOURCES}")

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
    ${PLATFORM_SOURCES}
    ${DOODLE_CORE_HEADERS}
    ${DOODLE_INCLUDE_HEADERS}
    ${DOODLE_INCLUDE_SOURCES}
    ${PLATFORM_HEADERS}
)

# also set the C standard to C99
set_property(TARGET doodle PROPERTY C_STANDARD 99)

target_include_directories(doodle PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PLATFORM_DIR}
)

# If the platform.cmake sets additional properties (like target_link_libraries calls),
# they will already be in effect after being included.
