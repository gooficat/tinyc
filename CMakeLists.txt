cmake_minimum_required(VERSION 3.10)
project(tinyc LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/*.c)

add_executable(tinyc
	${SOURCES}
)

target_include_directories(tinyc PRIVATE ${CMAKE_SOURCE_DIR})

set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	if(NOT MSVC)
		target_compile_options(tinyc PRIVATE -g -O0 -Wall -Wextra -Wpedantic -Werror)
	else()
		target_compile_options(tinyc PRIVATE /Zi /W4 /Od)
	endif()
else()
	if(NOT MSVC)
		target_compile_options(tinyc PRIVATE -O2 -Wall -Wextra -Wpedantic -Werror)
	else()
		target_compile_options(tinyc PRIVATE  /W4 /O2)
	endif()
endif()