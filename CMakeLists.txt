cmake_minimum_required(VERSION 3.28)

project(http-server-cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

add_executable(server ${SOURCE_FILES})