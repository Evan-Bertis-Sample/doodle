message(STATUS "Configuring for native platform...")

# add sources and headers to PLATFORM_SOURCES and PLATFORM_HEADERS
file(GLOB PLATFORM_SOURCES
    "${PLATFORM_DIR}/*.c"
)

file(GLOB PLATFORM_HEADERS
    "${PLATFORM_DIR}/*.h"
)

# add the platform directory to the include directories
target_include_directories(${PLATFORM_NAME} PUBLIC ${PLATFORM_DIR})
target_sources(${PLATFORM_NAME} PRIVATE ${PLATFORM_SOURCES})

if(WIN32) # to mingw work as all the others
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif(WIN32)

set(CIMGUI_DIR ${PLATFORM_DIR}/cimgui)

# general settings
if(EXISTS ${CIMGUI_DIR}/imgui/backends)
    message(STATUS "Using imgui/backends")
    set(BACKENDS_DIR ${CIMGUI_DIR}/imgui/backends)
else()
    message(STATUS "Using backends")
    set(BACKENDS_DIR ${CIMGUI_DIR}/backends)
endif()

if(EXISTS ${CIMGUI_DIR}imgui/imgui_tables.cpp)
    set(TABLES_SOURCE ${CIMGUI_DIR}/imgui/imgui_tables.cpp)
else()
    set(TABLES_SOURCE "")
endif()

add_definitions("-DIMGUI_USER_CONFIG=\"${CIMGUI_DIR}/cimconfig.h\"")
add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")
add_definitions("-DIMGUI_DISABLE_DEMO_WINDOWS")

set(IMGUI_SOURCES
    ${CIMGUI_DIR}/cimgui.cpp
    ${CIMGUI_DIR}/imgui/imgui.cpp
    ${CIMGUI_DIR}/imgui/imgui_draw.cpp
    ${CIMGUI_DIR}/imgui/imgui_demo.cpp
    ${CIMGUI_DIR}/imgui/imgui_widgets.cpp
    ${CIMGUI_DIR}/imgui/imgui_tables.cpp
    ${TABLES_SOURCE}
)

set(IMGUI_LIBRARIES "")

if(WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
elseif(WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" ")
endif(WIN32)

add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")

# optional adding freetype
option(IMGUI_FREETYPE "add Freetype2" OFF)

if(IMGUI_FREETYPE)
    FIND_PACKAGE(freetype REQUIRED PATHS ${FREETYPE_PATH})
    list(APPEND IMGUI_LIBRARIES freetype)
    list(APPEND IMGUI_SOURCES ${CIMGUI_DIR}/imgui/misc/freetype/imgui_freetype.cpp)
    add_definitions("-DCIMGUI_FREETYPE=1")
endif(IMGUI_FREETYPE)

# dx11
list(APPEND IMGUI_SOURCES ${BACKENDS_DIR}/imgui_impl_dx11.cpp) 

# GLFW
list(APPEND IMGUI_SOURCES ${BACKENDS_DIR}/imgui_impl_glfw.cpp)

set(GLFW_VERSION 3.3.8)
include(FetchContent)
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/${GLFW_VERSION}.tar.gz)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

# set language 
add_library(cimgui STATIC ${IMGUI_SOURCES})

list(APPEND IMGUI_LIBRARIES glfw d3d11 d3dcompiler)

target_include_directories(cimgui PUBLIC ${CIMGUI_DIR}/imgui)
target_link_libraries(cimgui ${IMGUI_LIBRARIES})

set_target_properties(cimgui PROPERTIES C_STANDARD 99)
set_target_properties(cimgui PROPERTIES CXX_STANDARD 11)
set_target_properties(cimgui PROPERTIES LINKER_LANGUAGE CXX)

# using library
target_compile_definitions(${PLATFORM_NAME} PUBLIC -DCIMGUI_USE_DX11 -DCIMGUI_USE_GLFW)
target_link_libraries(${PLATFORM_NAME} 
    PUBLIC cimgui doodle
)

# Copy the DLL next to "display.exe"
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:glfw>"              # The actual DLL produced by the glfw target
        "$<TARGET_FILE_DIR:display>"       # The folder where display.exe was generated
)