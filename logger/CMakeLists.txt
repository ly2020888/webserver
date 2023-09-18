cmake_minimum_required(VERSION 3.8)
project(logger)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
add_library(logger)
target_sources(logger PRIVATE logger.cpp)
target_include_directories(logger PRIVATE ${INCPATH})
target_link_directories(logger PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(logger PUBLIC timer fmt::fmt pthread)