cmake_minimum_required(VERSION 3.10)

project(lox-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

option(EXPORT_COMPILE_COMMANDS_JSON "Export compile_commands.json" ON)

if (EXPORT_COMPILE_COMMANDS_JSON)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(src)
