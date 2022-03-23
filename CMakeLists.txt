cmake_minimum_required(VERSION 3.10)
project(git-verify-pack)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++2a -g -fno-omit-frame-pointer -fsanitize=address")
# set(CMAKE_CXX_FLAGS_DEBUG "-std=c++2a -g -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++2a -O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#include_directories(${PROJECT_SOURCE_DIR}/src)

# debug 模式
SET(CMAKE_BUILD_TYPE "Debug")

FILE (GLOB src_files ./src/*.cpp)

add_executable(a.out ${src_files})

# target_link_libraries(crc32c)

# enable_testing()
# add_test(NAME xxx_test COMMAND xxx_test)
