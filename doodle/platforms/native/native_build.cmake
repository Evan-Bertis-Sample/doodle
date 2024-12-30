message(STATUS "Configuring for native platform...")

# add native_doodle.c to DOODLE_CORE_SOURCES
message(STATUS "Adding native_doodle.c to the project")

# add all the .h files to DOODLE_CORE_HEADERS
message(STATUS "Adding all .h files to DOODLE_CORE_HEADERS")
file(GLOB_RECURSE NATIVE_DOODLE_HEADERS "${PLATOFRM_DIR}/*.h")

foreach(HEADER ${NATIVE_DOODLE_HEADERS})
    list(APPEND DOODLE_CORE_HEADERS ${HEADER})
endforeach()


# add all the .c files to DOODLE_CORE_SOURCES
message(STATUS "Adding all .c files to DOODLE_CORE_SOURCES")
file(GLOB_RECURSE NATIVE_DOODLE_SOURCES "${PLATFORM_DIR}/*.c")

foreach(SOURCE ${NATIVE_DOODLE_SOURCES})
    list(APPEND DOODLE_CORE_SOURCES ${SOURCE})
endforeach()


set(IMGUI_DIR "${PLATFORM_DIR}/imgui")

# now add all the .h/.hpp files to DOODLE_CORE_HEADERS from imgui
# get the list of all .h/.hpp files in imgui
file(GLOB_RECURSE IMGUI_HEADERS "${IMGUI_DIR}/*.h" "${IMGUI_DIR}/*.hpp")

# add all the .h/.hpp files to DOODLE_CORE_HEADERS
foreach(HEADER ${IMGUI_HEADERS})
    list(APPEND DOODLE_CORE_HEADERS ${HEADER})
endforeach()

# add all the .c/.cpp files to DOODLE_CORE_SOURCES from imgui
# get the list of all .c/.cpp files in imgui
file(GLOB_RECURSE IMGUI_SOURCES "${IMGUI_DIR}/*.c" "${IMGUI_DIR}/*.cpp")

# add all the .c/.cpp files to DOODLE_CORE_SOURCES
foreach(SOURCE ${IMGUI_SOURCES})
    list(APPEND DOODLE_CORE_SOURCES ${SOURCE})
endforeach()
