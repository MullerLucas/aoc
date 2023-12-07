cmake_minimum_required(VERSION 3.27.8)

set(target_name aoc_23)

project(${target_name} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE sources ${source_dir}/*.cpp)

include_directories(./src)

add_executable(${target_name} ${sources})

if (MSVC)
    target_compile_options(${target_name} PRIVATE /W4 /WX)
else ()
    target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG -g)

