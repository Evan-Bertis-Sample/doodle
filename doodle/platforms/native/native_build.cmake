message(STATUS "Configuring for native platform...")

# add sources and headers to PLATFORM_SOURCES and PLATFORM_HEADERS
file(GLOB PLATFORM_SOURCES
    "${PLATFORM_DIR}/*.c"
)

file(GLOB PLATFORM_HEADERS
    "${PLATFORM_DIR}/*.h"
)

# include the cimgui library
add_subdirectory(${PLATFORM_DIR}/cimgui)