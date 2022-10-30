cmake_minimum_required(VERSION 3.8.1)
project(Horologion)

# UNIX variable is set TRUE for Cygwin and Apple systems
# Unfortunately need to first check if UNIX then exit if Apple or Cygwin

if(UNIX)
    message(STATUS "Detected *nix platform. Proceeding.")

    if (APPLE)
        message(FATAL_ERROR "Detected Apple *nix platform. Cannot proceed!")
    endif(APPLE)

    if (CYGWIN)
        message(FATAL_ERROR "Detected Cygwin *nix platform. Cannot proceed!")
    endif(CYGWIN)

else()
    message(FATAL_ERROR "Cannot compile on non-*nix platform!")

endif(UNIX)

message(STATUS "Detected ${CMAKE_SYSTEM}")

# Use C++ 17 or later
set(CMAKE_CXX_STANDARD 17)

# Lots of warnings + all warnings as errors
add_compile_options(-Wall -Wextra -pedantic -Werror)

# Relax when it comes to missing field initializers
add_compile_options(-Wno-missing-field-initializers)

# Support threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Set binary name
set(OUTPUT_NAME horolog)

# Set full path to binary
set(FULL_PATH_BINARY "${CMAKE_BINARY_DIR}/${OUTPUT_NAME}")

# Compile with debug symbols
set(CMAKE_BUILD_TYPE RelWithDebInfo)

message(STATUS "Compiled binary will be ${FULL_PATH_BINARY}")

include_directories("src")

add_executable(
    ${OUTPUT_NAME}
    "src/main.cpp"
    "src/command_base.cpp"
    "src/command_loop.cpp"
    "src/command_plan.cpp"
    "src/command_test.cpp"
    "src/parse_config_file.cpp"
    "src/utils_file.cpp"
    "src/utils_time.cpp"
    "src/utils_sysfs.cpp"
    "src/logger.cpp"
    "src/workers.cpp"
)

target_link_libraries(${OUTPUT_NAME} PRIVATE Threads::Threads)
