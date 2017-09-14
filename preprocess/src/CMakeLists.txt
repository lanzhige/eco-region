set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_PROGRAM_PATH "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/lib")

file(GLOB hdrs "*.h")
file(GLOB srcs "main.cpp")


add_executable(preprocess ${hdrs} ${srcs})