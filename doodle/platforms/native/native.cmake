message(STATUS "Configuring for native platform...")
target_compile_definitions(doodle_core PRIVATE DOODLE_NATIVE=1)
