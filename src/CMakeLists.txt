set(SRCS
  lox.cpp
  file.cpp
  error.cpp
  scanner.cpp
  parser.cpp
  ast_printer.cpp
)

add_executable(lox ${SRCS})

set(AST_DEFINES_JSON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ast_defines.json")
set(AST_DEFINES_INC_OUTPUT_DIR "${CMAKE_BINARY_DIR}/include")
set(AST_DEFINES_INC_PATH "${AST_DEFINES_INC_OUTPUT_DIR}/ast_defines.inc")
set(AST_INC_GENERATOR "${PROJECT_SOURCE_DIR}/scripts/ast_defines_json_to_inc.py")

file(MAKE_DIRECTORY "${AST_DEFINES_INC_OUTPUT_DIR}")

add_custom_command(
  OUTPUT "${AST_DEFINES_INC_PATH}"
  DEPENDS "${AST_DEFINES_JSON_PATH}"
  DEPENDS "${AST_INC_GENERATOR}"
  COMMAND "python3" "${AST_INC_GENERATOR}" "${AST_DEFINES_JSON_PATH}" "${AST_DEFINES_INC_PATH}"
)

add_custom_target(
  AST_DEFINES_INC
  DEPENDS "${AST_DEFINES_INC_PATH}"
)

add_dependencies(lox AST_DEFINES_INC)

target_include_directories(lox PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(lox PUBLIC "${CMAKE_BINARY_DIR}/include")
