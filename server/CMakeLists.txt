cmake_minimum_required(VERSION 3.8)
project(server)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
add_library(server)
target_sources(server PRIVATE
  clients.cpp server.cpp http.cpp)
target_include_directories(server PRIVATE ${INCPATH})
target_link_directories(server PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(server PRIVATE logger)