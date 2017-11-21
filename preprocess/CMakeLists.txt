cmake_minimum_required(VERSION 3.1)

option(BUILD_PREPROCESS "preprocessing" ON)

if (BUILD_PREPROCESS)
project(preprocess)
endif()
find_package(Boost REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${Boost_INCLUDE_DIRS}")

link_directories("${PROJECT_SOURCE_DIR}/lib")
link_directories("${Boost_LIBRARY_DIRS}")
set(CMAKE_PROGRAM_PATH "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/lib")

add_subdirectory(src)
