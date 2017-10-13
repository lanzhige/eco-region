set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "--std=c++11 -pthread -lpthread ${CMAKE_CXX_FLAGS}")


find_package(libbsoncxx REQUIRED)
find_package(libmongocxx REQUIRED)

file(GLOB hdrs "*.h")
file(GLOB srcs "main.cpp")

add_executable(preprocess ${hdrs} ${srcs})
target_link_libraries(preprocess "/home/lzhan253/project/eco-region/preprocess/bin/libshp.so" "/home/lzhan253/project/eco-region/preprocess/lib/libjsoncpp.a" ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES})
install(TARGETS preprocess DESTINATION .)
