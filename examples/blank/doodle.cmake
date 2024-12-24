# This will be called from the top-level doodle CMakeLists.txt
# it will be called using "include" and will be provided:
# PLATFORM_NAME
# PROJECT_NAME
# PROJECT_CMAKE_DIR
# PROJECT_SOURCE_DIR

function(build_project)
    message(STATUS "Building blank example")
    # find all of the .c files in this directory,
    # they will be compiled into the blank executable

    message(STATUS "Adding blank.c to the project, using path ${DOODLE_PROJECT_ROOT_DIR}/src/blank.c")

    # link blank.c to PROJECT_NAME executable, which has already been defined
    target_sources(${PROJECT_NAME} PRIVATE ${DOODLE_PROJECT_ROOT_DIR}/src/blank.c)

    message(STATUS "Linking doodle library")
    target_link_libraries(blank PRIVATE doodle)
endfunction()