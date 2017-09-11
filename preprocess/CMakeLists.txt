cmake_minimum_required(VERSION 3.1)

option(BUILD_PREPROCESS "preprocessing" ON)

if (BUILD_PREPROCESS)
project(preprocess)
endif()

add_subdirectory(src)