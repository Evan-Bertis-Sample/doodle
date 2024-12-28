message(STATUS "Configuring for native platform...")

# add native_doodle.c to DOODLE_CORE_SOURCES
message(STATUS "Adding native_doodle.c to the project")
list(APPEND DOODLE_CORE_SOURCES "${PLATFORM_DIR}/native_doodle.c")