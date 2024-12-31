message(STATUS "Configuring for native platform...")

# add sources and headers to PLATFORM_SOURCES and PLATFORM_HEADERS
file(GLOB PLATFORM_SOURCES
    "${PLATFORM_DIR}/*.c"
)

file(GLOB PLATFORM_HEADERS
    "${PLATFORM_DIR}/*.h"
)


set(CIMGUI_DIR ${PLATFORM_DIR}/cimgui)

# general settings
if (EXISTS ${CIMGUI_DIR}/imgui/backends)
    message(STATUS "Using imgui/backends")
	set(BACKENDS_DIR ${CIMGUI_DIR}/imgui/backends)
else()
    message(STATUS "Using backends")
	set(BACKENDS_DIR ${CIMGUI_DIR}/backends)
endif()

if (EXISTS ${CIMGUI_DIR}imgui/imgui_tables.cpp)
	set(TABLES_SOURCE ${CIMGUI_DIR}/imgui/imgui_tables.cpp)
else()
	set(TABLES_SOURCE "")
endif()

include_directories(${CIMGUI_DIR}/imgui)
add_definitions("-DIMGUI_USER_CONFIG=\"../cimconfig.h\"")
add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")

include_directories(${CIMGUI_DIR})
set(IMGUI_SOURCES 
${CIMGUI_DIR}/cimgui.cpp 
${CIMGUI_DIR}/imgui/imgui.cpp 
${CIMGUI_DIR}/imgui/imgui_draw.cpp 
${CIMGUI_DIR}/imgui/imgui_demo.cpp 
${CIMGUI_DIR}/imgui/imgui_widgets.cpp
${TABLES_SOURCE} 
)

set(IMGUI_LIBRARIES "")

if (WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
else(WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" ")
endif(WIN32)

add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")

# optional adding freetype
option(IMGUI_FREETYPE "add Freetype2" OFF)

if(IMGUI_FREETYPE)
	FIND_PACKAGE(freetype REQUIRED PATHS ${FREETYPE_PATH})
	list(APPEND IMGUI_LIBRARIES freetype)
	list(APPEND IMGUI_SOURCES ../../imgui/misc/freetype/imgui_freetype.cpp)
	add_definitions("-DCIMGUI_FREETYPE=1")
endif(IMGUI_FREETYPE)

# dx11
list(APPEND IMGUI_SOURCES ${BACKENDS_DIR}/imgui_impl_dx11.cpp)

if(WIN32)
       list(APPEND IMGUI_LIBRARIES opengl32)
else(WIN32) # Unix
       list(APPEND IMGUI_LIBRARIES GL)
endif(WIN32)

# GLFW
list(APPEND IMGUI_SOURCES ${BACKENDS_DIR}/imgui_impl_glfw.cpp)

set(GLFW_VERSION 3.3.8)
include(FetchContent)
	FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/${GLFW_VERSION}.tar.gz)

FetchContent_GetProperties(glfw)
if (NOT glfw_POPULATED)
	set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(glfw)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	if (NOT STATIC_BUILD)
	set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
	endif()
	add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

install(TARGETS glfw RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
					LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
#FIND_PACKAGE(glfw3 PATHS "C:/LuaGL/gitsources/BUILDS/GLFW/install")

# set language 


if (NOT STATIC_BUILD)
  add_library(cimgui SHARED ${IMGUI_SOURCES})
else()
  add_library(cimgui STATIC ${IMGUI_SOURCES})
endif()

target_link_libraries(cimgui ${IMGUI_LIBRARIES} glfw)
set_target_properties(cimgui PROPERTIES C_STANDARD 99)
set_target_properties(cimgui PROPERTIES CXX_STANDARD 11)
set_target_properties(cimgui PROPERTIES LINKER_LANGUAGE C)

# using library
include_directories(${CIMGUI_DIR}/generator/output/)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DCIMGUI_USE_DX11 -DCIMGUI_USE_GLFW)
target_link_libraries(${PROJECT_NAME} PRIVATE d3d11 d3dcompiler.lib cimgui)