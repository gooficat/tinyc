cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})

project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_CURRENT_SOURCE_DIR}/bin>")

include_directories(${CMAKE_SOURCE_DIR})
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

if(NOT DEFINED _CRT_SECURE_NO_WARNINGS)
  set(_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0) # -Wall -ansi)
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE /Zi /Od) # /W4 /Za)
  endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -O3) # -Wall -ansi)
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE /Ox) # /W4 /Za)
  endif()
endif()
