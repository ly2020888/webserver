cmake_minimum_required(VERSION 3.16)

project(timer)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

add_library(timer STATIC)

# target_
target_include_directories(timer PRIVATE ${INCPATH})
target_sources(timer PRIVATE timer.cpp)