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

# Set binary name
set(OUTPUT_NAME horolog)

# Set full path to binary
set(FULL_PATH_BINARY "${CMAKE_BINARY_DIR}/${OUTPUT_NAME}")

message(STATUS "Compiled binary will be ${FULL_PATH_BINARY}")

include_directories("src")

add_executable(
    ${OUTPUT_NAME}
    "src/main.cpp"
    "src/command_line_interface.cpp"
    "src/logger.cpp"
    "src/file_utils.cpp"
    "src/time_utils.cpp"
)