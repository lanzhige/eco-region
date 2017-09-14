set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")

file(GLOB hdrs "*.h")
file(GLOB srcs "main.cpp")

add_executable(preprocess ${hdrs} ${srcs})
target_link_libraries(preprocess shp)
install(TARGETS preprocess DESTINATION .)