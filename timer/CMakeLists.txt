cmake_minimum_required(VERSION 3.8)
project(timer)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.cpp)
include_directories(${INCPATH})

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

add_library(timer STATIC ${SRC})
