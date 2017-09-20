set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "--std=c++11 -pthread -lpthread ${CMAKE_CXX_FLAGS}")


#find_package(libbsoncxx REQUIRED)
#find_package(libmongocxx REQUIRED)

#include_directories("${LIBMONGOCXX_INCLUDE_DIR}/mongocxx/v_noabi")
#include_directories("${LIBBSONCXX_INCLUDE_DIR}/bsoncxx/v_noabi")

file(GLOB hdrs "*.h")
file(GLOB srcs "main.cpp")

add_executable(preprocess ${hdrs} ${srcs})
target_link_libraries(preprocess "/home/lzhan253/project/eco-region/preprocess/bin/libshp.so" "/home/lzhan253/project/eco-region/preprocess/lib/libjsoncpp.a" "/home/lzhan253/eco-region/preprocess/bin/libmongocxx.so")
install(TARGETS preprocess DESTINATION .)
