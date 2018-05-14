cmake_minimum_required(VERSION 3.0)

project(PARSER)
set (PARSER_VERSION_MAJOR 0)
set (PARSER_VERSION_MINOR 2)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PARSER_SRC ${CMAKE_SOURCE_DIR}/src)
include_directories("${PROJECT_SOURCE_DIR}/src/include")

if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "clang"))
  set (PEDANTIC_COMPILE_FLAGS -Wall -Wextra -Wshadow -pedantic)
endif()

add_executable (parser
    ${PARSER_SRC}/main.cc
    ${PARSER_SRC}/cfg.cc
    ${PARSER_SRC}/cfg_graph.cc
    ${PARSER_SRC}/cfg_reduction.cc
    ${PARSER_SRC}/regex_lex.cc
    ${PARSER_SRC}/regex_parser.cc
    )

