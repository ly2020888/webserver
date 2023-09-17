cmake_minimum_required(VERSION 3.8)
project(logger)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.cpp)

include_directories(${INCPATH})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_libraries(timer fmt::fmt pthread)
add_library(logger ${SRC})
