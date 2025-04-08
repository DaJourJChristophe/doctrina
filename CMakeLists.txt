cmake_policy(SET CMP0048 NEW)

cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_C_STANDARD_REQUIRED OFF)

set(CMAKE_C_FLAGS "-std=c99 -march=native -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_DEBUG "-fsanitize=address -fno-omit-frame-pointer -O3 -ggdb3")
set(CMAKE_C_FLAGS_RELEASE "-s -O3")

set(CMAKE_CXX_FLAGS "-std=c++20 -march=native -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address -fno-omit-frame-pointer -O3 -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-s -O3")

project(graphics VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_BUILD_DIR "${PROJECT_SOURCE_DIR}/build")
set(CMAKE_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/libexec")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_DIR}")

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/lib/cmocka/include")

link_directories("${PROJECT_SOURCE_DIR}/libexec")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ClangFormat)
# include(ClangTidy)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/test")
