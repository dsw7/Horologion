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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_compile_options(-Wall -Wextra -pedantic -Werror)

# Relax when it comes to missing field initializers
add_compile_options(-Wno-missing-field-initializers)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# -----------------------------------------------------------------------------------------------------------
# toml++

message(STATUS "Checking for toml++ header file")

set(TOMLPLUSPLUS_HPP "${CMAKE_SOURCE_DIR}/external/toml.hpp")

if(NOT EXISTS ${TOMLPLUSPLUS_HPP})
  message(STATUS "Downloading toml.hpp")
  file(
    DOWNLOAD
    "https://raw.githubusercontent.com/marzer/tomlplusplus/master/toml.hpp"
    ${TOMLPLUSPLUS_HPP}
    SHOW_PROGRESS
  )
else()
  message(STATUS "toml.hpp already exists. Skipping download")
endif()

# -----------------------------------------------------------------------------------------------------------
# {fmt}

message(STATUS "Locating {fmt} library")
find_package(fmt)

if(NOT fmt_FOUND)
  message(FATAL_ERROR "Is the {fmt} package installed?")
endif()

# -----------------------------------------------------------------------------------------------------------

include_directories(
  "src"
  "external"
)
add_executable(
  "horolog"
  "src/command_loop.cpp"
  "src/command_plan.cpp"
  "src/command_test.cpp"
  "src/configs.cpp"
  "src/files.cpp"
  "src/logger.cpp"
  "src/main.cpp"
  "src/utils.cpp"
  "src/workers.cpp"
)

target_link_libraries("horolog" PRIVATE Threads::Threads "fmt::fmt")

set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local")
install(TARGETS "horolog" DESTINATION "bin")
