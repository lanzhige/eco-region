set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "--std=c++11 -pthread -lpthread ${CMAKE_CXX_FLAGS}")

find_package(LIBBSONCXX REQUIRED)
find_package(LIBMONGOCXX REQUIRED)

file(GLOB hdrs "*.h")
file(GLOB srcs "main.cpp" "jsoncpp.cpp")

add_executable(preprocess ${hdrs} ${srcs})
target_link_libraries(preprocess  ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS preprocess DESTINATION .)
