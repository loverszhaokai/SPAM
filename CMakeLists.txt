# usage
# debug
# 2 make output dir: e.g. build-cmake/Debug
# 3 exec: cmake -H. -B./build-cmake/Debug/obj -DCMAKE_BUILD_TYPE=Debug
# 4 build target: cmake --build ./build-cmake/Debug/obj --target tt -- -j4
# 5 clean: cmake --build ./build-cmake/Debug --target clean

# release
# 1 enter baseparser dir
# 2 make output dir: e.g. build-cmake/Release
# 3 exec: cmake -H. -B./build-cmake/Release/obj -DCMAKE_BUILD_TYPE=Release
# 4 build target: cmake --build ./build-cmake/Release/obj --target tt -- -j4
# 5 clean: cmake --build ./build-cmake/Release --target clean

cmake_minimum_required(VERSION 3.3)
project (SPAM)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build-cmake/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build-cmake/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build-cmake/${CMAKE_BUILD_TYPE}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -D_DEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 \
    -DDYNAMIC_ANNOTATIONS_ENABLED=0 -DNDEBUG -DNVALGRIND")

add_executable(transform src/transform.cc)
