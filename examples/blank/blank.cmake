# This will be called from the top-level doodle CMakeLists.txt
# it will be called using "include" and will be provided:
# PLATFORM_NAME
# PROJECT_NAME
# PROJECT_CMAKE_DIR
# PROJECT_SOURCE_DIR

# this will have a function build_project that will be called

function(use_doodle_main)
    # returns if we are using the define DOODLE_MAIN
    add_definitions(-DDOODLE_MAIN)
endfunction()


function(build_project)
    message(STATUS "Building blank example")
    # find all of the .c files in this directory,
    # they will be compiled into the blank executable

    message(STATUS "Adding blank.c to the project, using path ${DOODLE_PROJECT_ROOT_DIR}/blank.c")

    add_executable(blank ${DOODLE_PROJECT_ROOT_DIR}/blank.c)

    message(STATUS "Linking doodle library")
    target_link_libraries(blank PRIVATE doodle)
endfunction()


